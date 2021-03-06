# include <cmath>
# include <vector>
# include <string>
# include <fstream>

# include "geometry.hpp"

std::vector<float> Geometry::linspace(float xi, float xf, int n)
{
    std::vector<float> linspaced;
    
    if (n == 0) return linspaced;
    if (n == 1)
    {
        linspaced.push_back(xi);
        return linspaced;
    } 

    linspaced.reserve(n);

    float delta = (xf - xi) / (n - 1);

    for (int i = 0; i < n; i++)
    {
        linspaced.emplace_back(xi + (float)(delta*i));
    }

    return linspaced;
}

void Geometry::set_SW(float x, float y) {SW.x = x; SW.y = y;}

void Geometry::set_NW(float x, float y) {NW.x = x; NW.y = y;}

void Geometry::set_SE(float x, float y) {SE.x = x; SE.y = y;}

int * Geometry::setGridPoints(float * p, int n, float dh, int nb)
{
    int * np = new int[n]();

    for (int i = 0; i < n; i++) np[i] = (int)(p[i] / dh) + nb;

    return np;
}

void Geometry::setGridShots()
{
    shots.all = shots.n_xline * shots.n_yline;

    shots.x = new float[shots.all];
    shots.y = new float[shots.all];
    shots.z = new float[shots.all];

    std::vector<float> x = linspace(SW.x, SE.x, shots.n_xline);
    std::vector<float> y = linspace(SW.y, NW.y, shots.n_yline);

    for (int k = 0; k < y.size(); k++)
    {
        for (int j = 0; j < x.size(); j++)
        {
            shots.x[j + k*x.size()] = x[j];
            shots.y[j + k*x.size()] = y[k];
            shots.z[j + k*x.size()] = shots.elevation;
        }
    }    

    std::vector< float >().swap(x);
    std::vector< float >().swap(y);
}

void Geometry::setGridNodes()
{
    nodes.all = nodes.n_xline * nodes.n_yline;     

    nodes.x = new float[nodes.all];
    nodes.y = new float[nodes.all];
    nodes.z = new float[nodes.all];

    std::vector<float> x = linspace(SW.x, SE.x, nodes.n_xline);
    std::vector<float> y = linspace(SW.y, NW.y, nodes.n_yline);

    for (int k = 0; k < y.size(); k++)
    {
        for (int j = 0; j < x.size(); j++)
        {
            nodes.x[j + k*x.size()] = x[j];
            nodes.y[j + k*x.size()] = y[k];
            nodes.z[j + k*x.size()] = nodes.elevation;
        }
    }    

    std::vector< float >().swap(x);
    std::vector< float >().swap(y);
}

void Geometry::setCircularShots()
{
    std::vector<float> x, y;

    for (float radius : shots.offsets)
    {
        float theta = 0.0f;

        while (theta < 2.0f * 4.0f*atan(1.0f))
        {            
            x.push_back(radius*sin(theta) + shots.xcenter);        
            y.push_back(radius*cos(theta) + shots.ycenter);        

            theta += acos(1.0f - powf(shots.circle_spacing,2.0f)/(2.0f*powf(radius,2.0f)));    
        }
    }

    shots.all = x.size();

    shots.x = new float[shots.all]();
    shots.y = new float[shots.all]();
    shots.z = new float[shots.all]();

    for (int i = 0; i < shots.all; i++)
    {
        shots.x[i] = x[i]; 
        shots.y[i] = y[i];
        shots.z[i] = shots.elevation;
    }

    std::vector< float >().swap(x);
    std::vector< float >().swap(y);
}

void Geometry::setCircularNodes()
{
    std::vector<float> x, y;

    for (float radius : nodes.offsets)
    {
        float theta = 0.0f;

        while (theta < 2.0f * 4.0f*atan(1.0f))
        {
            x.push_back(radius*sin(theta) + nodes.xcenter);        
            y.push_back(radius*cos(theta) + nodes.ycenter);        

            theta += acos(1.0f - powf(nodes.circle_spacing,2.0f)/(2.0f*powf(radius,2.0f)));    
        }
    }

    nodes.all = x.size();

    nodes.x = new float[nodes.all]();
    nodes.y = new float[nodes.all]();
    nodes.z = new float[nodes.all]();

    for (int i = 0; i < nodes.all; i++)
    {
        nodes.x[i] = x[i]; 
        nodes.y[i] = y[i];
        nodes.z[i] = nodes.elevation;
    }

    std::vector< float >().swap(x);
    std::vector< float >().swap(y);
}

void Geometry::setReciprocity()
{
    float * x = new float[shots.all];
    float * y = new float[shots.all];
    float * z = new float[shots.all];

    for (int p = 0; p < shots.all; p++)
    {
        x[p] = shots.x[p];
        y[p] = shots.y[p];
        z[p] = shots.z[p];
    }    

    delete[] shots.x;
    delete[] shots.y;
    delete[] shots.z;

    shots.x = new float[nodes.all];
    shots.y = new float[nodes.all];
    shots.z = new float[nodes.all];

    for (int p = 0; p < nodes.all; p++)
    {
        shots.x[p] = nodes.x[p];
        shots.y[p] = nodes.y[p];
        shots.z[p] = nodes.z[p];
    }    

    delete[] nodes.x;
    delete[] nodes.y;
    delete[] nodes.z;

    nodes.x = new float[shots.all];
    nodes.y = new float[shots.all];
    nodes.z = new float[shots.all];

    for (int p = 0; p < shots.all; p++)
    {
        nodes.x[p] = x[p];
        nodes.y[p] = y[p];
        nodes.z[p] = z[p];
    }    

    int aux = shots.all; shots.all = nodes.all; nodes.all = aux;   

    delete[] x;
    delete[] y;
    delete[] z;
}

void Geometry::exportPositions()
{
    std::ofstream shotsFile(shotsPath);        
    std::ofstream nodesFile(nodesPath);

    for (int node = 0; node < nodes.all; node++)        
    {   
        nodesFile <<nodes.x[node]<<", "<<nodes.y[node]<<", "<<nodes.z[node]<<std::endl;
    }

    for (int shot = 0; shot < shots.all; shot++)        
    {   
        shotsFile <<shots.x[shot]<<", "<<shots.y[shot]<<", "<<shots.z[shot]<<std::endl;    
    }

    shotsFile.close();
    nodesFile.close();
}
