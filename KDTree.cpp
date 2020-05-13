//
//  KDTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 8/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

class Point2D
{
public:
    Point2D () = default;
    
    Point2D(double x, double y) : xOrd(x), yOrd(y) {}
    
    bool operator<(const Point2D& p) const {
        if (this->yOrd < p.yOrd)
            return true;
        else if (this->yOrd > p.yOrd)
            return false;
        else
        {
            if (this->xOrd < p.xOrd)
                return true;
            else
                return false;
        }
    }
    
    double getX() const { return xOrd; }
    double getY() const { return yOrd; }
    
    bool operator==(const Point2D& p) const {
        return this->yOrd == p.yOrd && this->xOrd == p.xOrd;
    }
    
    double distanceSquared(const Point2D& p) const
    {
        double dx = this->xOrd - p.getX();
        double dy = this->yOrd - p.getY();
        return dx*dx + dy*dy;
    }
    
private:
    double xOrd;
    double yOrd;
};

class RectHV
{
public:
    RectHV(double xmin, double ymin, double xmax, double ymax)
        : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {}
    
    bool contains(const Point2D& p) const {
        if (p.getX() > xmax)
            return false;
        if (p.getX() < xmin)
            return false;
        if (p.getY() > ymax)
            return false;
        if (p.getY() < ymin)
            return false;
        return true;
    }
    
    double distanceSquaredTo(Point2D p) const {
        double dx = 0.0, dy = 0.0;
        if (p.getX() < xmin)
            dx = p.getX() - xmin;
        else if (p.getX() > xmax)
            dx = p.getX() - xmax;
        if (p.getY() < ymin)
            dy = p.getY() - ymin;
        else if (p.getY() > ymax)
            dy = p.getY() - ymax;
        return dx*dx + dy*dy;
    }
    
    bool intersects(const RectHV& that) const {
        return this->xmax >= that.xmin &&
               this->ymax >= that.ymin &&
               that.xmax >= this->xmin &&
               that.ymax >= this->ymin;
    }
    
    double getXMin() const { return xmin; }
    double getXMax() const { return xmax; }
    double getYMin() const { return ymin; }
    double getYMax() const { return ymax; }
    
private:
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};


class PointSet
{
public:
    PointSet() {}
    void add(const Point2D& p) { ordered_set.insert(p); }
    
    set<Point2D> rangeSearch(const RectHV& r)
    {
        auto itLow = std::lower_bound(ordered_set.cbegin(), ordered_set.cend(), Point2D(r.getXMin(), r.getYMin()));
        auto itHigh = std::upper_bound(ordered_set.cbegin(), ordered_set.cend(), Point2D(r.getXMax(), r.getYMax()));
        
        set<Point2D> ret;

        for(auto it = itLow; it != itHigh; it++)
        {
            if (r.contains(*it))
                ret.insert(*it);
        }
        return ret;
    }
    
    struct DistCompare
    {
        DistCompare(const Point2D& p) : benchMarkPoint(p) {}
        
        bool operator() (const Point2D& p1, const Point2D& p2) const {
            double d1 = benchMarkPoint.distanceSquared(p1);
            double d2 = benchMarkPoint.distanceSquared(p2);
            return d1 < d2;
        }
        
    private:
        Point2D benchMarkPoint;
    };
    Point2D nearestPoint(const Point2D& p)
    {
        //double dMinDistance = std::numeric_limits<double>::max();
        set<Point2D, DistCompare> distSet(p);
        for (auto& up : ordered_set)
        {
            distSet.insert(up);
        }
        auto it = distSet.cbegin();
        return *it;
    }
    
private:
    set<Point2D> ordered_set;
};

struct PointNode
{
    PointNode(const Point2D& p) : point(p), left(NULL), right(NULL) {}
    ~PointNode() {
        delete left;
        delete right;
    }
    
    Point2D point;
    PointNode* left;
    PointNode* right;
    
};

class TwoDTree
{
public:
    void add(const Point2D& p)
    {
        root = addNode(root, p, 0);
    }
    
    set<Point2D> rangeSearch(const RectHV& r)
    {
        double dMin = std::numeric_limits<double>::min();
        double dMax = std::numeric_limits<double>::max();
        set<Point2D> ret;
        dfsTraverseRange(r, root, 0, dMin, dMin, dMax, dMax, ret);
        
        return ret;
    }
    
    
    Point2D nearestPoint(const Point2D& p)
    {
        Point2D ret;
        
        double dMin = std::numeric_limits<double>::min();
        double dMax = std::numeric_limits<double>::max();
        dfsTraverseNearest(p, root, 0, dMin, dMin, dMax, dMax, dMax, ret);
        
        return ret;
    }
    
private:
    PointNode* root = NULL;
    
    PointNode* addNode(PointNode* curr, const Point2D& p, int level)
    {
        if (curr == NULL)
        {
            curr = new PointNode(p);
            return curr;
        }
        
        // If the point already exists, quit.
        if (curr->point == p)
            return curr;
        
        if (level % 2 == 0)
        {
            if (p.getX() < curr->point.getX())
                curr->left = addNode(curr->left, p, level+1);
            else
                curr->right = addNode(curr->right, p, level+1);

        }
        else
        {
            if (p.getY() < curr->point.getY())
                curr->left = addNode(curr->left, p, level+1);
            else
                curr->right = addNode(curr->right, p, level+1);
        }
        
        return curr;
    }
    
    void dfsTraverseRange(const RectHV& r, PointNode* curr, int level,
                          double xmin, double ymin, double xmax,
                          double ymax, set<Point2D>& output)
    {
        if (curr == NULL)
            return;
        
        if (r.contains(curr->point))
            output.insert(curr->point);
        
        if (level % 2 == 0)
        {
            if (r.intersects(RectHV(xmin, ymin, curr->point.getX(), ymax)))
                dfsTraverseRange(r, curr->left, level + 1, xmin, ymin, curr->point.getX(), ymax, output);
            if (r.intersects(RectHV(curr->point.getX(), ymin, xmax, ymax)))
                dfsTraverseRange(r, curr->right, level + 1, curr->point.getX(), ymin, xmax, ymax, output);
        }
        else
        {
            if (r.intersects(RectHV(xmin, ymin, xmax, curr->point.getY())))
                dfsTraverseRange(r, curr->left, level + 1, xmin, ymin, xmax, curr->point.getY(), output);
            if (r.intersects(RectHV(xmin, curr->point.getY(), xmax, ymax)))
                dfsTraverseRange(r, curr->right, level + 1, xmin, curr->point.getY(), xmax, ymax, output);
        }
        
    }
    
    double dfsTraverseNearest(const Point2D& p, PointNode* curr, int level,
                              double xmin, double ymin, double xmax,
                              double ymax, double dMinDistance, Point2D& output)
    {
        if (curr == NULL)
            return dMinDistance;
        
        double dDist = curr->point.distanceSquared(p);
        if (dDist < dMinDistance)
        {
            output = curr->point;
            dMinDistance = dDist;
        }
        
        if (level % 2 == 0)
        {
            double dLeftDistance = RectHV(xmin, ymin, curr->point.getX(), ymax).distanceSquaredTo(p);
            double dRightDistance = RectHV(curr->point.getX(), ymin, xmax, ymax).distanceSquaredTo(p);
            
            if (dLeftDistance < dRightDistance)
            {
                if (dLeftDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->left, level+1, xmin, ymin, curr->point.getX(), ymax, dMinDistance, output);
                if (dRightDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->right, level+1, curr->point.getX(), ymin, xmax, ymax, dMinDistance, output);
            }
            else
            {
                if (dRightDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->right, level+1, curr->point.getX(), ymin, xmax, ymax, dMinDistance, output);
                if (dLeftDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->left, level+1, xmin, ymin, curr->point.getX(), ymax, dMinDistance, output);
            }
        }
        else
        {
            double dLeftDistance = RectHV(xmin, ymin, xmax, curr->point.getY()).distanceSquaredTo(p);
            double dRightDistance = RectHV(xmin, curr->point.getY(), xmax, ymax).distanceSquaredTo(p);
            
            if (dLeftDistance < dRightDistance)
            {
                if (dLeftDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->left, level+1, xmin, ymin, xmax, curr->point.getY(), dMinDistance, output);
                if (dRightDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->right, level+1, xmin, curr->point.getY(), xmax, ymax, dMinDistance, output);
            }
            else
            {
                if (dRightDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->right, level+1, xmin, curr->point.getY(), xmax, ymax, dMinDistance, output);
                if (dLeftDistance < dMinDistance)
                    dMinDistance = dfsTraverseNearest(p, curr->left, level+1, xmin, ymin, xmax, curr->point.getY(), dMinDistance, output);
            }
        }
        
        return dMinDistance;
    }
};

TEST(PointSet, NoData)
{
    PointSet ps;
    TwoDTree tt;
    ifstream file ("input100K.txt");
    while (file.good())
    {
        double x, y;
        file >> x >> y;
        
        ps.add(Point2D(x, y));
        tt.add(Point2D(x, y));
    }
    
    
    auto start = std::chrono::high_resolution_clock::now();
    set<Point2D> ret = ps.rangeSearch(RectHV(0.1, 0.1, 0.45, 0.45));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    cout << "Point2D set range time difference: " << diff.count() << "\n";
    
    start = std::chrono::high_resolution_clock::now();
    set<Point2D> ret1 = tt.rangeSearch(RectHV(0.1, 0.1, 0.45, 0.45));
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    cout << "2D tree range time difference: " << diff.count() << "\n";
    EXPECT_EQ(ret, ret1);
    
    start = std::chrono::high_resolution_clock::now();
    Point2D p = ps.nearestPoint(Point2D(0.5, 0.55));
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    cout << "Point2D set nearest time difference: " << diff.count() << "\n";
    
     start = std::chrono::high_resolution_clock::now();
    Point2D p1 = tt.nearestPoint(Point2D(0.5, 0.55));
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    cout << "2D tree nearest time difference: " << diff.count() << "\n";
    
    EXPECT_EQ(p, p1);

}
