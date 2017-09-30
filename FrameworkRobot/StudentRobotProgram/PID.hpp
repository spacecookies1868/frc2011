/*
 *  PID.hpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/29/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#ifndef PID_HPP
#define PID_HPP

class PID
{
public:
    
    PID(double p, double i, double d)
    : m_sumError(0)
    , m_prevError(0)
    , m_kP(p)
    , m_kI(i)
    , m_kD(d)
    {}
    
    
    double Compute(double inError, double delta_time)
    {
        double deltaError = (inError - m_prevError) / delta_time;
        m_sumError += inError;
        m_prevError = inError;
        return (m_kP * inError) + (m_kI * m_sumError) + (m_kD * deltaError);
    }
    
private:
    
    double m_sumError;
    double m_prevError;
    
    double m_kP;
    double m_kI;
    double m_kD;
    
};

#endif


