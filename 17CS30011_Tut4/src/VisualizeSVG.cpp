#include "VisualizeSVG.hpp"

int VisualizeSVG::i = 0;

VisualizeSVG::VisualizeSVG(const string &_filepath) : filepath(_filepath)
{
    ofstream file;
    file.open (filepath);

    file << "<svg width = \"1000\" height = \"1000\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
    file << "<rect x = \"10\" y = \"10\" width = \"" << WIDTH + 2*MARGIN << "\" height = \"" << HEIGHT + 2*MARGIN << "\" style = \"fill:rgb(255,255,255); stroke-width:" << MARGIN << "; stroke:rgb(255,255,255)\"/>" << endl;
    
    file.close();
}

VisualizeSVG::VisualizeSVG()
{
    filepath = "t4#" + to_string(i++) + ".svg";

    ofstream file;
    file.open (filepath);

    file << "<svg width = \"1000\" height = \"1000\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
    file << "<rect x = \"10\" y = \"10\" width = \"" << WIDTH + 2*MARGIN << "\" height = \"" << HEIGHT + 2*MARGIN << "\" style = \"fill:rgb(255,255,255); stroke-width:" << MARGIN << "; stroke:rgb(255,255,255)\"/>" << endl;
    
    file.close();
}

void VisualizeSVG::draw_points(const vector<Point2D> &p)
{
    ofstream file;
    file.open (filepath, ios::app);
       
    for(int i = 0; i < p.size(); i++)
        file << "<circle cx = \"" << p[i].x << "\" cy = \"" << p[i].y << "\" r = \"" << RADIUS << "\" stroke = \"black\" stroke-width = \"0\" fill = \"black\" fill-opacity = \"1\"/>" << endl;

    file.close();
}

void VisualizeSVG::draw_line(const vector<double> &x, const vector<double> &y)
{
    ofstream file;
    file.open(filepath, ios::app);

    file << "<line x1 = \"" << x[0] << "\" y1 = \"" << y[0] << "\" x2 = \"" << x[1] << "\" y2 = \"" << y[1] << "\" stroke = \"lightpink\" stroke-opacity = \"1\"/>" << endl;
    
    file.close();
}

void VisualizeSVG::draw_circle(const Point2D &c, const double &radius)
{
    ofstream file;
    file.open(filepath, ios::app);

    file << "<circle cx = \"" << c.x << "\" cy = \"" << c.y << "\" r = \"" << radius << "\" stroke = \"lightblue\" stroke-width = \"0\" fill = \"lightblue\" fill-opacity = \"0.5\"/>" << endl;

    file.close();
}

void VisualizeSVG::end_file()
{
    ofstream file;
    file.open(filepath, ios::app);

    file << "</svg>" << endl;

    file.close();
}
