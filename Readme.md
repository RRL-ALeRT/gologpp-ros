# Installation

Clone this repository to a ROS2 workspace's src folder and make sure the internet is connected.
Build the package in the workspace folder with:
```
colcon build --symlink-install --packages-select gologpp_agent
```

## For Blocksworld example in webots with spot and manipulator, run following commands in terminal one by one. Use separate tab for each.
```
ros2 launch webots_spot spot_launch.py
```
```
ros2 launch webots_spot moveit_launch.py
```
```
ros2 service call /Spot/blocksworld_pose webots_spot_msgs/srv/SpotMotion
ros2 run webots_spot gpp_blocksworld_server
```
```
ros2 launch webots_spot blocksworld_launch.py
```
Profit!