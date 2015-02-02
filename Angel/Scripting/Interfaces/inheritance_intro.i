%include <factory.i>

%factory(Actor*, TextActor, FullScreenActor, MazeFinder, HUDActor, PhysicsActor, Camera, ParticleActor);
%factory(DeveloperLog*, ConsoleLog, CompoundLog, FileLog, SystemLog);
%factory(MessageListener*, TextActor, MazeFinder, FullScreenActor, HUDActor, Camera, PhysicsActor, ParticleActor, Actor);
%factory(Renderable*, TextActor, FullScreenActor, MazeFinder, Camera, HUDActor, PhysicsActor, Actor, GridActor, ParticleActor);