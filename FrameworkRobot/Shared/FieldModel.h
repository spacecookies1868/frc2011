/*
 *  FieldModel.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FIELD_MODEL_H
#define FIELD_MODEL_H
/**
 * Class which defines field characteristics.
 * @author Eric Bakan
 */
class FieldModel
{
public:
    
    // distance is in meters
    
    virtual double  GetFieldLength() const { return 16.46; }
    virtual double  GetFieldWidth() const { return 8.23; }
    virtual double  GetRollingResistance() const   {return .001;}
    virtual ~FieldModel() {}
};
#endif
