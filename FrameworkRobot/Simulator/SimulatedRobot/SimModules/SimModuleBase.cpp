/*
 *  SimModuleBsae.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimModuleBase.h"
#include <list>
#include <assert.h>

typedef std::list<SimModuleBase*>	ModuleList;

SimModuleBase::SimModuleBase()
{
	assert(s_ModuleUpdateList.empty());
	s_AllModules.push_back(this);
}

void SimModuleBase::UpdateAllModules(uint32_t simTimeMS, double delta_simTimeSec)
{
	for (ModuleList::iterator it=s_ModuleUpdateList.begin(); it != s_ModuleUpdateList.end(); ++it)
		(*it)->Update(simTimeMS, delta_simTimeSec);
}

void SimModuleBase::BuildModuleNetwork()
{
	for (ModuleList::iterator it=s_AllModules.begin(); it != s_AllModules.end(); ++it)
		AddToUpdateList(*it);
}

void SimModuleBase::DeleteAllModules()
{
	for (ModuleList::iterator it=s_AllModules.begin(); it != s_AllModules.end(); ++it)
		delete (*it);
	s_AllModules.clear();
	s_ModuleUpdateList.clear();
}

bool SimModuleBase::ExistsInList(const SimModuleBase* pModule, const ModuleList& moduleList)
{
	for (ModuleList::const_iterator it=moduleList.begin(); it != moduleList.end(); ++it)
		if ((*it) == pModule)
			return true;
	return false;
}

void SimModuleBase::AddToUpdateList(SimModuleBase* pModule)
{
	for (ModuleList::iterator it=pModule->m_sources.begin(); it != pModule->m_sources.end(); ++it)
		if (!ExistsInList(*it, s_ModuleUpdateList))
			AddToUpdateList(*it);
	
	if (!ExistsInList(pModule, s_ModuleUpdateList))
		s_ModuleUpdateList.push_back(pModule);
}