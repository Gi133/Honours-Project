# Honours-Project
Goal Oriented and Personality AI for Background NPCs

## How to run
Simply download the whole thing (yes, unfortunately), build and run. I currently don't own a website so I have nowhere to upload the executable itself.

## Where is your code, not the engine stuff.
I modified some parts of the engine code, but those were to make the backend easier to eventually thread. This included modifications and update to the random number generator. Otherwise, all the "game" code can be found in the ClientGame folder.

## Goal Oriented AI
The agents in the world operate using goals. The main goal for these trading NPCs is to make money. Because I didn't wish to include pathfinding through subgoals/atomicgoals, all composite goals actually include a list of subgoals to go along with them. This means that the "Make Money" goal is also the trade goal that will queue up a trade, including buying and selling the resources as well as moving to the city to buy or sell resources.

The majority of these goals can fail and there are parts of the goal code that handle such fail states. In the case of a sell or purchase failing (usually due to change in prices between the time the NPC queried the price and arriving at the city) then it will keep the resources in its inventory and attempt to sell them later on. This behavior can be extended but the attention was in making this code appropriate for background NPCs where they are not afforded much computation time.

### What makes a Goal?
As mentioned above, the AI works through goals. These goals come in two flavours, these are Atomic and Composite. A good explanation of this system can be found in the book by Matt Buckland called "Programming AI by Example".
Atomic goals exist on their own and are used to perform basic actions such as moving from one city to another. On the other hand, Composite Goals include their own Atomic, or even Composite, sub-goals and are used for more complex goals such as Trading.

Each goal requires its own Evaluator, these are queried to score the desirability of their goal. This score is then taken and multiplied by the agents bias towards the said goal. The goal with the highest score is then queued to the Agent goal list.

## Other than AI, what else?
Everything had to be developed in the application, the main use of the engine was to avoid programming the graphics, file/settings and messaging systems as well as taking advantage of the building script. As such, some major features developed include:
### Economy Simulation
The economy is simulated along with the world itself. Every set amount of ingame time the city population as well as resource change randomly based on the current values. For example, population might increase or decrease and so on. This, in turn affects the price the city is willing to pay to purchase or sell items based on the following formula.

ResourcePrice=BasePrice+(((CityPopulation/CurrentCityStock)/CurrentCityStock)×BasePrice×PriceMultiplier)

### Trading
This leads to the trading mechanics, trading is done through a saved list of average prices in the world. This is also done to improve the time it takes for an agent to find a trade and get going. A good trade is a trade where resource is bought below the global average price and sold above it. This does introduce a problem where the economy can balance out if huge shifts in population and pricing are not made, but for the most part works well and keeps the average compuation time for each agent near 0.2ms.

### Inventory System
The Inventory system was developed with the idea of it being robust enough to allow for further expansion. Currently, it consists of two parts. The first is the Purse, which can be upgraded to increase the maximum number of gold an NPC can hold. Lastly, the Bag system itself is included. An agent can have a number of bags, each existing indipendantly in such a way that the removal of this bag will remove only what was included in it. This opens up potentials for different random events where the bags can rip open and their contents lost without having to randomly decide what exactly will disappear from a more homogenous inventory system. Much like the purses, these bags can individually be upraded and more bags purchased by the NPCs.

### User Interface
The user interface is rather primitive but some background research was done in how user interfaces are usually developed. The UI is layered, the top layer is the UI window, these can then include various UI Elements. Each Window can be of variable size and anchored in one of the 9 cardinal positions of the application window. Some work was done in allowing for windows to be resized along with the application window but this was scrapped after being notified that the application should be resolution locked. The UI Element currently consists of two parts, the Title and Content, both of which simply take in some text and display it. It is possible to extend this to show other things aswell.
An interesting feature of this UI is that Elements can be dynamically added or removed and their size and position will change depending on the size of the window they reside in.

### Time Simulation
In the game world, mere seconds can count the passage of days or months. This is done in order to allow for some realism in how the world changes and how it operates. Actions such as buying or selling resources take at least a day to complete (this can be easily modified in the application) and other tasks such as moving between cities take into account the passage of time and the speed of the agent to provide this added level of simulation.

### World Generation
For the purpose of showing how dynamic the AI can be, the world is randomly generated every time the application is launched. The random generation is done for the locations on the map such as cities and dungeons as well as their starting numbers of resources or threat respectively. The generator works with a rather simple algorithm as the aim was to allow for a random economic situation instead of a more visual representation. Exposed in the options file are the maximum number of cities and dungeons on the map along with the map dimensions. Alongside these values, the radii for cities and dungeons can also be found. The cities are placed on the map first, the rule behind this is that no city should be within the radius of another city already on the map. If this check fails a set number of times then it moves to the second phase of the map generation where the dungeons are placed on the map. These dungeons operate using the same rule but instead of simply checking against city radii they also check against those of already placed dungeons on the map. This means that dungeons and cities do not end up clustered and are instead spread throughout most of the map depending on the settings provided.

Locations are also assigned names, different pools of possible names are used for cities and dungeons (both of which are included in their own text files to which additions can be made and dynamically included in the application). Alongside a name they are also given their starting population and resource stocks or threat level depending on whether it is a city or a dungeon.

It should be noted that the map shown in the UI is resized to fit the map window.

### Name Manager
The name manager is a simple manager that loads in set text files that include names for agents, dungeons and cities. These are them used to provide full agent names or location names according to the situation. These names can be unique, which will put them in a pool of used names that is checked every time a new name is requested or they can be common in which case this check is not carried out. As mentioned above, these text files can be edited freely as long as there is atleast 1 name set. Each name in the files should be in its own line for the parser to parse it properly. Once these names are loaded into memory they are then ready to be called at any time.

### Resource Manager
The resource manager as a very flashy name and is used extensively for the purpose of initially reading in the resource file containing the resource names and their base values but also to turn these resources into easy to use values in the code in order to stop the need for looking up something like "stone". It provides functionality to type in the words "Stone" or "sTone" or any other variation and it will execute a look up and provide the appropriate resource iterator number to be used throughout the application. Amongst this functionality there are also other helper functions included that are used extensively in code rated to fetching resource data or moving resources. This is done in order to provide a very safe way to look up for resources without requiring potentially unsafe code or rewriting the same checks on resource names.

Lastly, the resource manager keeps track of the global average price of each resource and also handles part of the trading logic by providing functionality to quickly get prices and trade routes from cities for specific resources as well as providing the trade route struct used in these algorithms. The trade route search comes in varying flavours. It allows to execute a global search for all or a specific resource from one city with no set destination while also allowing for more targeted searches where the origin and destination are known and the potential trade routes of all or specific resources are required. Additional logic check is provided for agent variables if the functionality is to be used by agents, such as threat level check or distance.

# Is that it?
This is a rather short overview of some of the things going on behind the scenes when the application is executed, further details can be seen in the code itself but those would mostly be to improve performance or to keep the code safe and bug-free where possible.

There is certainly still work that can be carried out to improve the interface and the agent functionality or add extra features. These may come depending on how much free time I have on my hands and whether any benefit can be gained from spending time in implementing such things. 

Overall the project's aim was to test whether this sort of AI approach could be used to give some extra realism to NPCs found in open world games and the average computation time of each agent was 0.2ms on the machine used as a test. It happened that the example chosen was trading as that was assumed to be a common sight, this provided extra difficulties and added to the whole computation of the goals, as a lot of these games tend feature multiple cities. Traders would need to connect these cities for them to realistically exist as they can not all always be self sustained and even then traders would still possibly be required. The resource manager makes this easy to change the resources given into anything required for the world theme. For example, the Wood resource could be renamed to Water and its price changed accordinly to simulate a different sort of world. Additional functionality will need to be implemented to the resource files to include their own potential price algorithms and changes over time in order to allow for extensive simulation of such scenarios however.
