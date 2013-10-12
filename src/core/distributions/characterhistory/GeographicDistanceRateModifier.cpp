//
//  GeographicDistanceRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <cmath>
#include "CharacterEVent.h"
#include "GeographicDistanceRateModifier.h"
#include "RbConstants.h"
#define EARTHRADIUSKM 6371

using namespace RevBayesCore;

GeographicDistanceRateModifier::GeographicDistanceRateModifier(std::vector<std::vector<double> > gc, double dp, double th, std::string dt) : geographicCoordinates(gc), threshhold(th), numAreas(gc.size()), distancePower(dp),distanceType(dt)
{
    
    geographicDistances.resize(numAreas);
    geographicDistancePowers.resize(numAreas);
    
    for (size_t i = 0; i < numAreas; i++)
    {
        geographicDistances[i].resize(numAreas,0.0);
        geographicDistancePowers[i].resize(numAreas,0.0);
    }
    for (size_t i = 0; i < numAreas; i++)
    {
        for (size_t j = i; j < numAreas; j++)
        {
            geographicDistances[i][j] = computePairwiseDistances(i,j);
            geographicDistances[j][i] = geographicDistances[i][j];
        }
    }
    
    updateGeographicDistancePowers(1.0,true);
    update();
    
    //printAll();
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    if (newState->getState() == 0)
        return 1.0;
    
    //std::cout << newState->getIndex() << " : ";
    
    // determine which areas are present and which are absent
    std::set<CharacterEvent*> present;
    std::set<CharacterEvent*> absent;
    for (size_t i = 0; i < numAreas; i++)
    {
        if (currState[i]->getState() == 0)
        {
            //std::cout << currState[i] << i << "," << currState[i]->getIndex() << " ";
            absent.insert(currState[i]);
        }
        else
            present.insert(currState[i]);
    }
    
    // get sum of distances_ij^beta
    double rate = 0.0;
    double sum = 0.0;
    std::set<CharacterEvent*>::iterator it_p;
    std::set<CharacterEvent*>::iterator it_a;
    for (it_p = present.begin(); it_p != present.end(); it_p++)
    {
        size_t idx_p = (*it_p)->getIndex();
        for (it_a = absent.begin(); it_a != absent.end(); it_a++)
        {
            size_t idx_a = (*it_a)->getIndex();
            double d = geographicDistancePowers[idx_p][idx_a];
            sum += d;
            
            if (idx_a == newState->getIndex())
                rate += d;
        }
    }
    
    // get sum-normalized rate-modifier
    double r = absent.size() * rate / sum;
    
    //std::cout << "drm   " << r << " = " << absent.size() << " * " << rate << " / " << sum <<  "\n";
    
    return r;
}

GeographicDistanceRateModifier* GeographicDistanceRateModifier::clone(void) const
{
    return new GeographicDistanceRateModifier(*this);
}



void GeographicDistanceRateModifier::update(void)
{
    for (size_t i = 0; i < numAreas; i++)
    {
        for (size_t j = i; j < numAreas; j++)
        {
            geographicDistancePowers[i][j] = pow(geographicDistances[i][j], -distancePower);
            geographicDistancePowers[j][i] = geographicDistancePowers[i][j];
        }
    }
}


void GeographicDistanceRateModifier::updateGeographicDistancePowers(double dp, bool upd)
{
    distancePower = dp;
    if (upd)
        update();
}

double GeographicDistanceRateModifier::computePairwiseDistances(int i, int j)
{
    double d = 0.0;
    double rad = RbConstants::PI/180;
    double lat0 = geographicCoordinates[i][0] * rad;
    double lat1 = geographicCoordinates[j][0] * rad;
    double lon0 = geographicCoordinates[i][1] * rad;
    double lon1 = geographicCoordinates[j][1] * rad;
    
    if (distanceType == "haversine")
    {
        //double d = 0.0;
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        double sindlat = sin(dlat/2);
        double sindlon = sin(dlon/2);
        double a = sqrt(sindlat * sindlat + cos(lat0) * cos(lat1) * sindlon * sindlon);
        if (a < 0.9995)
            d = 2 * asin(a) * EARTHRADIUSKM;
        else
            d = 2 * asin(1.0) * EARTHRADIUSKM;
    }
    else if (distanceType == "euclidean")
    {
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        d = pow(dlat*dlat+dlon*dlon, 0.5);
    }
    
    return d;
}

void GeographicDistanceRateModifier::print(std::vector<std::vector<double> > m)
{
    std::vector<std::vector<double> >::iterator it0;
    std::vector<double>::iterator it1;
    
    for (it0 = m.begin(); it0 != m.end(); it0++)
    {
        for (it1 = it0->begin(); it1 != it0->end(); it1++)
        {
            std::cout << *it1 << " ";
        }
        std::cout << "\n";
    }
}

void GeographicDistanceRateModifier::printAll(void)
{
    std::cout << "\ngc\n";
    print(geographicCoordinates);
        std::cout << "\ngd\n";
    print(geographicDistances);
        std::cout << "\ngdp\n";
    print(geographicDistancePowers);
    
}