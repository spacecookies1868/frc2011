/*
 *  untitled.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_MODULE_BASE_H
#define SIM_MODULE_BASE_H

#include "Shared/CommonIncludes.h"
#include <list>

class SimModuleBase
{
	typedef std::list<SimModuleBase*> ModuleList;
	public:
		SimModuleBase();
		virtual ~SimModuleBase() {}
		virtual void Update(uint32_t simTimeMS, double delta_simTimeSec) = 0;
		static void UpdateAllModules(uint32_t simTimeMS, double delta_simTimeSec);
		static void BuildModuleNetwork();
		static void DeleteAllModules();

	private:
		static bool	ExistsInList(const SimModuleBase* pModule, const ModuleList& moduleList);
		static void AddToUpdateList(SimModuleBase* pModule);

	protected:
		ModuleList	m_sources;

	public:
		static	ModuleList	s_AllModules;
		static	ModuleList	s_ModuleUpdateList;

};

#endif
