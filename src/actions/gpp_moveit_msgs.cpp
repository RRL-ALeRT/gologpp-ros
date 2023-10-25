#include "action_manager.h"
#include "exog_manager.h"
#include "ros_backend.h"

#include <execution/controller.h>

#include <moveit_msgs/action/move_group.hpp>

template<>
ActionManager<moveit_msgs::action::MoveGroup>::GoalT
ActionManager<moveit_msgs::action::MoveGroup>::build_goal(const gpp::Activity &a)
{
	auto agent_node = Singleton::instance();

	auto goal_msg = moveit_msgs::action::MoveGroup::Goal();
    auto motion_plan_request = moveit_msgs::msg::MotionPlanRequest();

    // Set up motion plan request and constraints
    motion_plan_request.workspace_parameters.header.stamp = agent_node->get_clock()->now();
    motion_plan_request.workspace_parameters.header.frame_id = "base_link";
    motion_plan_request.workspace_parameters.min_corner.x = -1.0;
    motion_plan_request.workspace_parameters.min_corner.y = -1.0;
    motion_plan_request.workspace_parameters.min_corner.z = -1.0;
    motion_plan_request.workspace_parameters.max_corner.x = 1.0;
    motion_plan_request.workspace_parameters.max_corner.y = 1.0;
    motion_plan_request.workspace_parameters.max_corner.z = 1.0;
    motion_plan_request.start_state.is_diff = true;

    moveit_msgs::msg::Constraints constraints;
    // Add joint constraints
    for (int i = 0; i < 6; ++i) {
        moveit_msgs::msg::JointConstraint jc;
        std::string joint_name = "joint_" + std::to_string(i + 1);
        jc.joint_name = joint_name;
        double position = static_cast<double>(a.mapped_arg_value(joint_name).numeric_convert<float>());
        jc.position = position * 3.14159 / 180;
        jc.tolerance_above = 0.001;
        jc.tolerance_below = 0.001;
        jc.weight = 1.0;
        constraints.joint_constraints.push_back(jc);
    }
    motion_plan_request.goal_constraints.push_back(constraints);

    // Set planning options
    motion_plan_request.pipeline_id = "ompl";
    motion_plan_request.group_name = "manipulator";
    motion_plan_request.num_planning_attempts = 4;
    motion_plan_request.allowed_planning_time = 10.0;
    motion_plan_request.max_velocity_scaling_factor = 0.4;
    motion_plan_request.max_acceleration_scaling_factor = 0.4;
    motion_plan_request.max_cartesian_speed = 0.0;

    moveit_msgs::msg::PlanningOptions planning_options;
    planning_options.plan_only = false;
    planning_options.look_around = true;
    planning_options.look_around_attempts = 5;
    planning_options.max_safe_execution_cost = 0.0;
    planning_options.replan = true;
    planning_options.replan_attempts = 4;
    planning_options.replan_delay = 0.1;
    // Fill in the goal message
    goal_msg.request = motion_plan_request;
    goal_msg.planning_options = planning_options;
	
	return goal_msg;
}

void RosBackend::define_moveit_actions()
{
	create_ActionManager<moveit_msgs::action::MoveGroup>("/move_action");
}