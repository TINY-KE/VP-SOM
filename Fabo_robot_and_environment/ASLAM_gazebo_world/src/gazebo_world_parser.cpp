#include <gazebo/gazebo.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/physics/physics.hh>
#include <iostream>

int main(int argc, char **argv)
{
    // Load Gazebo
    gazebo::setupClient(argc, argv);

    // Create a world and get the models
    gazebo::physics::WorldPtr world = gazebo::physics::get_world("default");
    gazebo::physics::Model_V models = world->GetModels();

    // Loop through each model and get its pose and size
    for (auto model : models) {
        // Get model pose
        gazebo::math::Pose pose = model->GetWorldPose();
        double x = pose.pos.x;
        double y = pose.pos.y;
        double z = pose.pos.z;

        // Get model size
        gazebo::physics::Shape_V shapes = model->GetShapes();
        for (auto shape : shapes) {
            gazebo::math::Vector3 size = shape->GetSize();
            double width = size.x;
            double height = size.y;
            double depth = size.z;

            // Print model information
            std::cout << "Model name: " << model->GetName() << std::endl;
            std::cout << "Position: x=" << x << " y=" << y << " z=" << z << std::endl;
            std::cout << "Size: width=" << width << " height=" << height << " depth=" << depth << std::endl;
            std::cout << std::endl;
        }
    }

    // Shutdown Gazebo
    gazebo::shutdown();

    return 0;
}
