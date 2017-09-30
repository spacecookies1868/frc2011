#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <time.h>
#include "Shared/CommonIncludes.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_thread.h"
#include "SimGUI/SimulatorWindow.h"
#include "SimGUI/SimHardware.h"
#include "SimGUI/SimulatorController.h"
#include "SimGUI/SimulatedProgram.h"
#include "SimulatedRobot/SimulatedField.h"
#include "FieldObject/FieldElements/FieldElements.h"

#include "SimulatedRobot/SimulatedRobotModel.h"
#include "StudentRobotProgram/StudentProgram.h"
#include "SimulatedRobot/SimulatedXBoxController.h"

using namespace std;

#ifdef _WIN32
#undef main
#endif

int main(int argc, char *argv[])
{ 
    // Create all simulation code
    SimulatorController* simCtlr = new SimulatorController();
    
    SimHardware* simHW = SimHardware::GetInstance();

    SimulatedField* sim_field = new SimulatedField();
	
	//Generate Field
	FieldObject::Alliance r = FieldObject::red;
	FieldObject::Alliance b = FieldObject::blue;
	
    //borders
	AllianceWall* alliancewall_left = new AllianceWall(Vector3(0,sim_field->GetFieldWidth()/2,0),Vector3(0,0,0), b, "Left Alliance Wall");
	sim_field->AddFieldObject(alliancewall_left);
	AllianceWall* alliancewall_right = new AllianceWall(Vector3(sim_field->GetFieldLength(),sim_field->GetFieldWidth()/2,0),Vector3(0,0,0), r, "Right Alliance Wall");
	sim_field->AddFieldObject(alliancewall_right);
	
	GuardRail* guardrail_top = new GuardRail(Vector3(sim_field->GetFieldLength()/2,sim_field->GetFieldWidth(),0),Vector3(0,0,0), "Top Guard Rail");
	sim_field->AddFieldObject(guardrail_top);
	GuardRail* guardrail_bottom = new GuardRail(Vector3(sim_field->GetFieldLength()/2,0,0),Vector3(0,0,0), "Bottom Guard Rail");
	sim_field->AddFieldObject(guardrail_bottom);
	
	
	//towers
	Tower* tower_blue_top = new Tower(Vector3(5.9,sim_field->GetFieldWidth()-1.3,0),Vector3(0,0,0),b,"Top Blue Tower");
	sim_field->AddFieldObject(tower_blue_top);
	Tower* tower_blue_bottom = new Tower(Vector3(5.9,1.3,0),Vector3(0,0,0),b,"Bottom Blue Tower");
	sim_field->AddFieldObject(tower_blue_bottom);
	
	Tower* tower_red_top = new Tower(Vector3(sim_field->GetFieldLength()-5.9,sim_field->GetFieldWidth()-1.3,0),Vector3(0,0,0),r,"Top Red Tower");
	sim_field->AddFieldObject(tower_red_top);
	Tower* tower_red_bottom = new Tower(Vector3(sim_field->GetFieldLength()-5.9,1.3,0),Vector3(0,0,0),r,"Bottom Red Tower");
	sim_field->AddFieldObject(tower_red_bottom);
	
	//zones
	ScoringZone* red_zone = new ScoringZone(Vector3(1.07,sim_field->GetFieldWidth()/2,0), Vector3(0,0,0), r, "Red Scoring Zone");
	sim_field->AddFieldObject(red_zone);
	ScoringZone* blue_zone = new ScoringZone(Vector3(sim_field->GetFieldLength()-1.07,sim_field->GetFieldWidth()/2,0), Vector3(0,0,0), b, "Blue Scoring Zone");
	sim_field->AddFieldObject(blue_zone);
	
	//lanes
	Lane* lane_blue_top = new Lane(Vector3(2.97,sim_field->GetFieldWidth()-.65,0),Vector3(0,0,0), b, "Top Blue Lane");
	sim_field->AddFieldObject(lane_blue_top);
	Lane* lane_blue_bottom = new Lane(Vector3(2.97,.65,0),Vector3(0,0,0), b, "Bottom Blue Lane");
	sim_field->AddFieldObject(lane_blue_bottom);
	Lane* lane_red_top = new Lane(Vector3(sim_field->GetFieldLength()-2.97,sim_field->GetFieldWidth()-.65,0),Vector3(0,0,0), r, "Top Red Lane");
	sim_field->AddFieldObject(lane_red_top);
	Lane* lane_red_bottom = new Lane(Vector3(sim_field->GetFieldLength()-2.97,.65,0),Vector3(0,0,0), r, "Bottom Red Lane");
	sim_field->AddFieldObject(lane_red_bottom);
	
	//scoring grids
	ScoringGrid* grid_red_top = new ScoringGrid(Vector3(0,sim_field->GetFieldWidth()/2,0), Vector3(0,0,0), r, "Top Red Scoring Grid");
	sim_field->AddFieldObject(grid_red_top);
	
	ScoringGrid* grid_blue_top = new ScoringGrid(Vector3(sim_field->GetFieldLength(),sim_field->GetFieldWidth()/2,0), Vector3(0,0,M_PI), b, "Top Blue Scoring Grid");
	sim_field->AddFieldObject(grid_blue_top);
	
    // Starting position for the robot
    double starting_width, starting_height, starting_angle;
    static const bool random_location_init=false;
    
    if (random_location_init)
    {
        srand(time(NULL));
        starting_width=(rand() & 0x1F)/32.0;
        srand(time(NULL));
        starting_height=(rand() & 0x1F)/32.0;
        srand(time(NULL));
        starting_angle=(rand() & 0x1F)/32.0*360;
    } else {
        starting_angle = 0;
        starting_height = 0.5;
        starting_width = 0.5;
    }
	
    const Vector3 initialLocation(sim_field->GetFieldLength()*starting_width, sim_field->GetFieldWidth()*starting_height,0);
    const Vector3 initialAngle(0,0,starting_angle);
	
    SimulatedRobotModel* pRobotModel = new SimulatedRobotModel(sim_field,initialLocation, initialAngle, FieldObject::blue);
    
    SimulatedXBoxDriveController* simXBoxCtlr = new SimulatedXBoxDriveController();
    
    StudentProgram* student_prog = new StudentProgram(pRobotModel, simXBoxCtlr);
    
    SimulatedProgram* sim_prog = new SimulatedProgram(student_prog, simCtlr);
    
    student_prog->setInitialPositionInformation(initialLocation, Vector3(), initialAngle, Vector3());
    
    
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    } else {
      fprintf(stdout, "SDL Initialized Correctly\n");
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    
    SimulatorWindow::StartGLScreen(false, pRobotModel, student_prog, sim_field, simCtlr);
    
    simHW->Initialize(pRobotModel, sim_field, simCtlr);
    simCtlr->Initialize(sim_prog, simXBoxCtlr);

    simCtlr->RunEventLoop();

    SimulatorWindow::FinishGLScreen();
    simHW->Destroy();
    
    delete simXBoxCtlr, simCtlr, sim_prog, student_prog, pRobotModel, sim_field;

    return EXIT_SUCCESS;
}
