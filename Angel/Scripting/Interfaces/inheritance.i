%include <factory.i>

%factory(Actor*, TextActor, HUDActor, FullScreenActor, Camera, PhysicsActor, ParticleActor);
%factory(DeveloperLog*, ConsoleLog, CompoundLog, FileLog, SystemLog);
%factory(MessageListener*, TextActor, FullScreenActor, Camera, HUDActor, PhysicsActor, ParticleActor, Actor);
%factory(Renderable*, TextActor, FullScreenActor, HUDActor, Camera, PhysicsActor, Actor, GridActor, ParticleActor);