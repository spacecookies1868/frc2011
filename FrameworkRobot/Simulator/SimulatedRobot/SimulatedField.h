/*
 *  SimulatedField.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/21/10.
 *
 */

#ifndef SIMULATED_FIELD_H
#define SIMULATED_FIELD_H

#include "Shared/FieldModel.h"
#include <list>
#include "FieldObject/FieldObject.h"

class SimulatedField : public virtual FieldModel
{
public:
	SimulatedField() {}
	~SimulatedField() {}
	int AddFieldObject(FieldObject* f);
	FieldObject* DeleteFieldObject(int index);
	const std::vector<FieldObject*>& GetFieldObjects() const {return m_fieldObjects;}
	void CheckCollisions();

private:
	std::vector<FieldObject*> m_fieldObjects;
};

#endif
