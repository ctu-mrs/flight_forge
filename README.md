# FlightForge: Advancing UAV Research with Procedural Generation of High-Fidelity Simulation and Integrated Autonomy

This repository contains the code for the **FlightForge: Advancing UAV Research with Procedural Generation of High-Fidelity Simulation and Integrated Autonomy** presented in our [paper](https://arxiv.org/abs/2502.05038v1) and the [project website](https://mrs.fel.cvut.cz/flight-forge). 
The documentation can be found [here](https://ctu-mrs.github.io/docs/simulations/FlightForge/).
The ROS package is available in the [ctu-mrs/mrs_uav_unreal_simulation](https://github.com/ctu-mrs/mrs_uav_unreal_simulation) repository.
For usage without ROS can be found in the [ctu-mrs/mrs_flight_forge_connector](https://github.com/ctu-mrs/mrs_flight_forge_connector) repository.

## Usage

This repository contains the code for the **FlightForge** simulation environment, which is based on Unreal Engine 5.
If you want to create custom worlds or edit the simulator, you need to have Unreal Engine 5.4.3 installed, if you want to use the simulator without editing it, you can use the precompiled version of the simulator it is downloadable here [MRS FlightForge simulator](https://nasmrs.felk.cvut.cz/index.php/s/MnGARsSwnpeVy5z)

1. Create a new C++ project in Unreal Engine 5.4.3
2. Clone this repository into the `Plugins` folder of your project, if it's not created create it.
```bash
git clone https://github.com/ctu-mrs/flight_forge.git 
```
3. Open the project in Unreal Engine 5.4.3 and compile the project.
4. Check the `Plugins` section in the project settings and enable the `FlightForge` plugin.
5. Set the Game Mode to `UedsGameMode` in the project settings.
6. Create a new level and check that the Game Mode is set to `UedsGameMode`.
7. Find the PawnSpawn and set its name to `UAV`
8. Try to play the game,  now you should be able to spawn the UAV in the world using the [ROS interface](https://github.com/ctu-mrs/mrs_uav_unreal_simulation) or the [C++ / Python interface](https://github.com/ctu-mrs/mrs_flight_forge_connector).


## Publication

If you use this work in your research, please cite the following paper:

```
@inproceedings{capek2025flightforge,
  title   = {FlightForge: Advancing UAV Research with Procedural Generation of High-Fidelity Simulation and Integrated Autonomy},
  author  = {David \v{C}apek, Jan Hrn\v{c}\'{i}\v{r}, Tom\'{a}\v{s} B\'{a}\v{c}a, Jakub Jirkal, Vojt\v{e}ch Von\'{a}sek, Robert P\v{e}ni\v{c}ka and Martin Saska
},
  year    = {2025},
  booktitle = {IEEE/RSJ International Conference on Robotics and Automation (ICRA)}, 
}
```
