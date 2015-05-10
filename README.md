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
The user interface is rather premitive but some background research was done in how user interfaces are usually developed. The UI is layered, the top layer is the UI window, these can then include various UI Elements. Each Window can be of variable size and anchored in one of the 9 cardinal positions of the application window. Some work was done in allowing for windows to be resized along with the application window but this was scrapped after being notified that the application should be resolution locked. The UI Element currently consists of two parts, the Title and Content, both of which simply take in some text and display it. It is possible to extend this to show other things aswell.
An interesting feature of this UI is that Elements can be dynamically added or removed and their size and position will change depending on the size of the window they reside in.
