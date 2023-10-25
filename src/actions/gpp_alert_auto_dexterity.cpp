#include "action_manager.h"
#include "ros_backend.h"

#include <execution/controller.h>

#include "alert_auto_dexterity/action/manipulator_manipulation.hpp"
#include "alert_auto_dexterity/action/stand_in_front.hpp"

template<>
ActionManager<alert_auto_dexterity::action::ManipulatorManipulation>::GoalT
ActionManager<alert_auto_dexterity::action::ManipulatorManipulation>::build_goal(const gpp::Activity &a)
{
	auto goal = alert_auto_dexterity::action::ManipulatorManipulation::Goal();
	goal.location = std::string(a.mapped_arg_value("location"));

	return goal;
}

template<>
ActionManager<alert_auto_dexterity::action::StandInFront>::GoalT
ActionManager<alert_auto_dexterity::action::StandInFront>::build_goal(const gpp::Activity &a)
{
	auto goal = alert_auto_dexterity::action::StandInFront::Goal();
	goal.location = std::string(a.mapped_arg_value("location"));

	return goal;
}

void RosBackend::define_alert_auto_dexterity_actions()
{
	create_ActionManager<alert_auto_dexterity::action::ManipulatorManipulation>("/see_with_manipulator");
	create_ActionManager<alert_auto_dexterity::action::ManipulatorManipulation>("/goto_estops_with_manipulator");
	create_ActionManager<alert_auto_dexterity::action::ManipulatorManipulation>("/press_estop");

	create_ActionManager<alert_auto_dexterity::action::StandInFront>("/stand_in_front");
}