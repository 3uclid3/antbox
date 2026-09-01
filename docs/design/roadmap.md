# antbox loose roadmap

This roadmap gives antbox a general order of development without assigning
dates or promising exact scope. Each stage should produce an observable,
interesting simulation and reveal something useful about the `ant` ECS
framework before the next stage begins.

The stages may overlap when convenient. Their order primarily reflects
dependencies: later behavior should build on world and inspection capabilities
that already work.

## Current foundation

antbox already has a fixed-timestep simulation, a working camera, colonies, and
moving ants. This is the visual and technical baseline rather than a disposable
prototype.

## 1. Make the simulation operable

Add the smallest useful workbench around the current ants.

- Pause and resume the simulation.
- Change simulation speed and advance by a single tick.
- Select and follow an ant or colony.
- Inspect the selected entity's components.
- Delete an entity and spawn a colony from the world context.
- Display basic performance measurements such as entity count and tick time.

**Outcome:** the existing simulation can be stopped, examined, modified, and
measured. These tools become the foundation for developing every later system.

## 2. Establish the world substrate

Introduce the spatial concepts that food, walls, pheromones, and an endless
world will share.

- Divide world space into deterministic chunks.
- Provide spatial lookup for nearby entities and world data.
- Add nests as explicit colony locations.
- Add simple food sources and walls.
- Add workbench tools for placing and removing world elements.
- Keep generation minimal, but make new chunks reproducible from a seed.

Only a small region needs to be active at first. The important decision is that
world data is chunk-addressable from the beginning, so endless generation does
not later require replacing a finite-world model.

**Outcome:** ants inhabit an editable world with meaningful locations and
resources.

## 3. Make pheromones visible and behavioral

Build pheromones as both simulation data and part of the normal visual
presentation.

- Store pheromone fields in world chunks.
- Deposit, sense, reinforce, and evaporate pheromone.
- Render pheromone using the same restrained visual language as the ants.
- Begin with the minimum useful channels, likely home and food.
- Add workbench controls for showing, hiding, painting, and erasing fields.
- Expose pheromone sensing in the selected ant's inspector.

**Outcome:** trails visibly form and dissolve, and ant motion begins to show
collective organization.

## 4. Close the survival loop

Connect movement, food, nests, population, and death into one self-sustaining
system.

- Ants search for food and carry it back to their nest.
- Colonies store food.
- Ants expend energy, age, and die.
- Colonies spend stored food to create new ants.
- Population rises or falls according to the colony's success.
- Inspection explains an ant's state, target, sensed signals, and steering.

Tune for readable behavior rather than ecological detail. Brood stages,
genetics, and specialized castes are not required to validate this loop.

**Outcome:** colonies can grow, stabilize, or collapse without scripted events.

## 5. Let colonies reproduce

Allow successful colonies to create daughter colonies through visible, risky
founding expeditions.

- A prosperous colony may invest in a princess.
- The princess recruits a small group of workers.
- The group leaves the parent colony and searches for a new site.
- Founding a nest turns the princess into a queen and transfers the workers to
  the daughter colony.
- Failed expeditions consume real resources and may die out.
- Preserve ancestry, with a subtle inherited variation in colony color.
- Record colony founding and extinction as observable events.

**Outcome:** colony count and territory emerge from the survival simulation
rather than from random spawning alone.

## 6. Make the world genuinely endless

Scale the established simulation beyond the area currently being observed.

- Generate chunks as the camera or living entities reach new space.
- Activate and deactivate chunks according to simulation activity.
- Preserve modified or otherwise persistent world state.
- Discard unchanged regions that can be reconstructed from the seed.
- Resolve pheromone decay and finite ant energy across chunk transitions.
- Add save and load once the persistent state is understood.
- Stress-test large populations, long runs, and repeated chunk activation.

**Outcome:** colonies can expand beyond everything previously explored while
the cost of the active simulation remains bounded.

## 7. Add pressures and deeper experiments

After the core world is interesting on its own, introduce systems that disturb
its equilibrium.

Possible directions include:

- competition when unrelated colonies meet;
- simple hostile insects represented by other geometric marks;
- danger pheromone and fleeing or fighting;
- food depletion, regeneration, and uneven distribution;
- additional terrain constraints;
- colony lineage and population history;
- charts, bookmarks, event history, and short replays; and
- richer workbench operations and simulation parameters.

These are options rather than a fixed checklist. Each should earn its place by
creating a new observable interaction or by applying useful pressure to `ant`.

**Outcome:** antbox becomes a deeper experimental environment without losing
the simplicity of its core simulation or graphics.

## Framework feedback throughout

Every stage is also a test of `ant`. During development, capture:

- APIs that make systems awkward to express or test;
- missing inspection and tooling capabilities;
- scheduling or data-model limitations;
- common application test fixtures that may belong in framework test support;
- performance limits and the workloads that expose them; and
- patterns that repeat often enough to justify framework-level support.

Framework changes should be driven by concrete pressure from antbox. The goal
is not to predict every abstraction in advance, but to let a real simulation
show what `ant` is missing.

