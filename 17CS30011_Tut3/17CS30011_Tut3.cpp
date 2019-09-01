/*************************
Generates SVG file for Closest Point pair algorithm using Divide and Conquer approach
*************************/
#include<bits/stdc++.h>
using namespace std;

#define HEIGHT 400
#define WIDTH 1000
#define MARGIN 30

int compareX(const pair<int, int> a, const pair<int, int> b)
{
    pair<int, int> p1 = a, p2 = b;
    return (p1.first - p2.first);
}

int compareY(const pair<int, int> a, const pair<int, int> b)
{
    pair<int, int> p1 = a, p2 = b;
    return (p1.second - p2.second);
}

double dist(pair<int, int> p1, pair<int, int> p2)
{
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

void gen_random(int num_points, vector<pair<int, int> > &Px, vector<pair<int, int> > &Py)
{
    int x, y;
    srand(time(NULL));
    vector<vector<int> > not_distinct(WIDTH + MARGIN + 5, vector<int>(HEIGHT + MARGIN + 5, 0));
    for(int i = 0; i < num_points; i++)
    {
        pair<int, int> p;
        p.first = rand()%WIDTH + MARGIN;
        p.second = rand()%HEIGHT + MARGIN;
        if(not_distinct[p.first][p.second])
        {
            i--;
            continue;
        }
        not_distinct[p.first][p.second] = 1;
        Px.push_back(p);
        Py.push_back(p);
    }
}

double closest_dist(vector<pair<int, int> > Px, vector<pair<int, int> > Py, int num_points)
{
    if(num_points <= 3)
    {
        cout << "h2" << endl;
        double min_dist = 1e9;
        for(int i = 0; i < num_points; i++)
            for(int j = i + 1; j < num_points; j++)
                if(dist(Px[i], Px[j]) < min_dist)
                    min_dist = dist(Px[i], Px[j]);
        return min_dist;
    }
    int mid = num_points / 2;

    pair<int, int> p_mid = Px[mid];
    vector<pair<int, int> > PyL(mid + 1);
    vector<pair<int, int> > PyR(num_points - mid - 1);
    int li = 0, ri = 0;
    for(int i = 0; i < num_points; i++)
    {
        if(Py[i].first <= p_mid.first)
            PyL[li++] = Py[i];
        else
            PyR[ri++] = Py[i];
    }

    vector<pair<int, int> > Px_mid;
    for(int i = mid; i < num_points; i++) Px_mid.push_back(Px[i]);

    cout << "h1" << endl;
    cout << num_points << endl;
    cout << mid << endl;        
    double min_l = closest_dist(Px, PyL, mid);
    double min_r = closest_dist(Px_mid, PyR, num_points - mid);
    // for(int i = mid; i < num_points; i++) Px_mid.push_back(Px[i]);
    double min_dist = min(min_l, min_r);
    cout << "h3" << endl;
    vector<pair<int, int> > del_strip;
    for(int i = 0; i < num_points; i++)
        if(abs(Py[i].first - p_mid.first) < min_dist)
            del_strip.push_back(Py[i]);

    for(int i = 0; i < del_strip.size(); i++)
        for(int j = i + 1; j < del_strip.size() && (del_strip[j].second - del_strip[i].second) < min_dist; j++)
            if(dist(del_strip[i], del_strip[j]) < min_dist)
                min_dist = dist(del_strip[i], del_strip[j]);

    return min_dist;
}

int main()
{
    int num_points;
    char choice;
    cout << "Enter R for to generate random points and M to enter points manually:\n";
    cin >> choice;
    cout << "Enter the number of points:\n";
    cin >> num_points;

    vector<pair<int, int> > Px, Py; 
    if(choice == 'R')
        gen_random(num_points, Px, Py);

    else
    {
        cout << "Enter the points as (x, y):\n";
        for(int i = 0; i < num_points; i++)
        {
            pair<int, int> p;
            cin >> p.first >> p.second;
            Px.push_back(p);
            Py.push_back(p);
        }
    }

    sort(Px.begin(), Px.end(), compareX);
    sort(Py.begin(), Py.end(), compareY);

    double min_dist = closest_dist(Px, Py, num_points);

    cout << "The minimum distance is: " << min_dist << "\n";
    return 0;
}