# include "model.hpp"

# include "../inout/inout.hpp"

void Model3D::init()
{
    this->nxx = this->nx + 2 * this->nb;
    this->nyy = this->ny + 2 * this->nb;
    this->nzz = this->nz + 2 * this->nb;

    this->nPoints = this->nx * this->ny * this->nz;
    this->nPointsB = this->nxx * this->nyy * this->nzz;
}

void Model3D::readAndExpandVP()
{
    this->init();

    this->vp = new float[this->nPointsB];

    float * model = new float[this->nPoints];

    InOut::readBinaryFloat(vpPath, model, this->nPoints);  

    // Centering
    for (int z = this->nb; z < this->nzz - this->nb; z++)
    {
        for (int y = this->nb; y < this->nyy - this->nb; y++)
        {
            for (int x = this->nb; x < this->nxx - this->nb; x++)
            {
                this->vp[z + x*this->nzz + y*this->nxx*this->nzz] = model[(z - this->nb) + (x - this->nb)*this->nz + (y - this->nb)*this->nx*this->nz];
            }
        }
    }

    // Z direction
    for (int z = 0; z < this->nb; z++)
    {
        for (int y = this->nb; y < this->nyy - this->nb; y++)
        {
            for (int x = this->nb; x < this->nxx - this->nb; x++)
            {
                this->vp[z + x*this->nzz + y*this->nxx*this->nzz] = model[0 + (x - this->nb)*this->nz + (y - this->nb)*this->nx*this->nz];
                this->vp[(this->nzz - z - 1) + x*this->nzz + y*this->nxx*this->nzz] = model[(this->nz - 1) + (x - this->nb)*this->nz + (y - this->nb)*this->nx*this->nz];
            }
        }
    }

    // X direction
    for (int x = 0; x < this->nb; x++)
    {
        for (int z = 0; z < this->nzz; z++)
        {
            for (int y = this->nb; y < this->nyy - this->nb; y++)
            {
                this->vp[z + x*this->nzz + y*this->nxx*this->nzz] = this->vp[z + this->nb*this->nzz + y*this->nxx*this->nzz];
                this->vp[z + (this->nxx - x - 1)*this->nzz + y*this->nxx*this->nzz] = this->vp[z + (this->nxx - this->nb - 1)*this->nzz + y*this->nxx*this->nzz];
            }
        }
    }

    // Y direction
    for (int y = 0; y < this->nb; y++)
    {
        for (int z = 0; z < this->nzz; z++)
        {
            for (int x = 0; x < this->nxx; x++)
            {
                this->vp[z + x*this->nzz + y*this->nxx*this->nzz] = this->vp[z + x*this->nzz + this->nb*this->nxx*this->nzz];
                this->vp[z + x*this->nzz + (this->nyy - y - 1)*this->nxx*this->nzz] = this->vp[z + x*this->nzz + (this->nyy - this->nb - 1)*this->nxx*this->nzz];
            }
        }
    }

    delete[] model;
}