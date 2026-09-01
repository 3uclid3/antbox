# antbox design vision

## Purpose

antbox is a living ant-colony simulation workbench built to exercise the
[`ant`](https://github.com/3uclid3/ant) ECS framework in a real application.
It should expose missing framework capabilities, provide realistic performance
pressure, and help guide the development of `ant`.

The simulation is the product. antbox is not intended to become a conventional
game.
There is no requirement for objectives, progression, rewards, or a win state.

The experience should invite a simple loop:

> Observe something interesting, form a hypothesis, perturb the world, and
> watch what happens.

## Product principles

- **Emergence over scripting.** Interesting behavior should arise from simple
  interacting systems.
- **Observable and editable.** The user should be able to inspect the
  simulation and interfere with it while it runs.
- **Endless, but locally alive.** The generated world may continue indefinitely,
  while active simulation remains bounded by living entities and colonies.
- **Primitive visuals, expressive motion.** Complexity should come from the
  simulation rather than graphical detail.
- **No decorative data.** Every mark in the world should represent something in
  the simulation.
- **The framework is part of the experiment.** Awkward application code,
  missing tooling, and performance limits are useful findings about `ant`.

## Core survival loop

The first complete simulation loop is:

> Ants leave the nest, search for food, reinforce pheromone trails, return food,
> and allow the colony to create new ants. Ants expend energy, age, and die;
> colonies grow, survive, or collapse.

This loop gives the other systems a shared purpose. Walls change routes, food
supports population, pheromones create collective behavior, and hostile insects
or other colonies threaten something that can be lost.

### Colonies

A colony has an identity, a nest, stored food, and a population. Every ant has a
colony association. Colonies can be founded and can eventually become extinct.

The nest may act as the colony's food store and source of new ants. A queen is
the reproductive center of the colony.

#### Colony founding

New colonies should be born from successful existing colonies rather than
appearing randomly. When a colony has enough food and population, it may invest
in a princess: a costly reproductive ant that can become a queen.

The princess recruits a small group of workers and leaves the parent colony.
The expedition travels beyond the parent's territory and searches for a
suitable site. When it establishes a nest, the princess becomes its queen and
the accompanying workers transfer their association to the daughter colony.

Founding has a real cost and can fail. The parent loses the food and workers it
invested, while an expedition that travels too far or loses its princess may
die before establishing a nest. Randomness can influence when the opportunity
arises, which direction the group travels, and which site it chooses, but every
new colony should have a visible cause and ancestry.

The daughter may inherit a subtle variation of the parent's color so that its
lineage remains visible. Detailed mating, males, brood stages, and genetics are
not required initially; the important behavior is the risky, observable split
of one colony into two.

### Ants

Ant behavior should be composed from small states and signals, such as:

- exploring;
- sensing and following pheromone;
- collecting food;
- returning to the nest;
- avoiding walls;
- fleeing or fighting; and
- expending energy, aging, and dying.

An ant that wanders too far can simply exhaust itself. This naturally limits
the active frontier without placing an artificial boundary around the world.

### Pheromones

Pheromones are a central simulation system and a mandatory part of the normal
visual presentation. Their formation, reinforcement, diffusion, and evaporation
should be visible because watching collective trails emerge is part of the
experience, not merely a debugging aid.

Possible channels include food, home, and danger. The system should begin with
the smallest set required for the survival loop.

### World

The world should be infinite in the Minecraft sense: moving beyond explored
space generates more world. Deterministic generation from a seed should allow
unchanged areas to be reconstructed.

World regions may be:

- actively simulated near living entities and colonies;
- dormant when persistent state exists but detailed simulation is unnecessary;
  or
- discarded when they contain no persistent changes and can be regenerated.

Pheromone decay and finite ant energy help regions return to an inactive state.

Initial world elements are food, nests, walls, ants, and colonies. Hostile
insects and competing colonies are later pressures on the survival loop.

## Workbench

The workbench should make the world understandable without turning into a game
HUD.

### Time

- pause and resume;
- simulation speed control;
- single-tick stepping; and
- deterministic restart from the same seed.

Playback history or time scrubbing may follow later if the simulation can
support it cleanly.

### Inspection

- select and follow entities;
- inspect entity components and current behavior;
- navigate from an ant to its colony;
- inspect colony population and stored food;
- search or filter entities; and
- show optional overlays for pheromones and other spatial data.

A particularly useful inspector would explain why an ant is moving as it is:
its current state, sensed signals, target, steering influences, and likely next
transition.

### Editing

- place or remove food and walls;
- spawn or remove colonies;
- move an entity;
- paint or erase pheromone; and
- alter selected component values.

Contextual actions should depend on what is selected. An ant may be deleted,
moved, or reassigned; a colony may receive food or spawn an ant; empty world
space may spawn a colony or place a world element.

### History and measurement

Useful later additions include simulation saves, bookmarks, an event history,
population and resource graphs, performance counters, screenshots, and short
replays. These serve both observation and the goal of stress-testing `ant`.

## Visual direction

The current ant establishes the desired tone: extremely simple, but with a feel
created by its motion.

The visual language should remain deliberately small:

- ants are tiny moving marks;
- food is a cluster of points;
- nests are circles, rings, or hollow points;
- walls are plain lines;
- hostile insects are simple triangles, squares, or other geometric marks;
- pheromones use restrained points, stippling, or a coarse translucent field;
  and
- selection uses a thin ring or corner marks.

There should be no need for sprites, textures, lighting, shadows, terrain
dressing, or decorative particles. State should be communicated economically
through motion, color, shape, or one additional mark. For example, an ant can
carry food as a contrasting dot.

At close range the user should see individual decisions. From farther away,
ants should merge into flows and colonies into evolving patterns. Visual polish
should come from timing, color relationships, transitions, and legibility—not
from added detail.

The workbench UI should respect the same restraint. The world occupies most of
the screen, panels appear when useful, selections remain subtle, and overlays
can be controlled independently.

## Initial vertical slice

The first version that can validate the idea should contain:

1. one colony and nest;
2. food patches;
3. exploration, collection, and return behavior;
4. visible food and home pheromones;
5. food-funded births and energy- or age-based deaths;
6. pause, speed, and single-step controls;
7. entity and colony selection and inspection; and
8. tools to place food, walls, colonies, and pheromone.

The important test is whether this small world creates moments of curiosity:
"Why are they doing that?" If it does, additional systems can deepen an already
interesting simulation. Competing colonies, hostile insects, ecology, and more
varied terrain can follow without redefining its core.
