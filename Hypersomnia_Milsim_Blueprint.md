# Blueprint: Transforming Hypersomnia into a Tactical Top-Down Milsim

This document outlines the core structural, mechanical, and architectural changes required to shift *Hypersomnia* from a fast-paced, competitive arena arcade game into a punishing, high-stakes tactical military simulation (Milsim). 

---

## 1. Information Denial & Dynamic Fog of War

In a true military simulation, information is the most lethal asset. The current $360^{\circ}$ omniscient screen visibility must be heavily restricted to simulate biological and tactical limitations.

```
         [ Enemy Invisible ]
             \       /
              \     /
               \   /
[ Enemy ] ───► [Player Cone: 90°-120°] ───► [ Enemy Visible ]
               /   \
              /     \
             /       \
         [ Enemy Invisible ]
```

###[DONE] Visual Cone Boundaries (Field of View) 
* **Mechanical Overhaul:** Bind the rendering camera's visibility matrices directly to the player's vector head/weapon orientation.
* **The Constraints:** Restrict the active visual field to a **$90^{\circ}$ to $120^{\circ}$ forward cone**. Any entities, map sprites, or enemy characters outside this directional arc must be completely culled from the client's screen.
* **Tactical Execution:** Forces players to manually "slice the pie" when clearing corners, exposing their presence pixel by pixel rather than instantly catching visual frames around hard angles.

###[NEEDS REVIEW] Auditory Mapping & Sound Propagation
* **Mechanical Overhaul:** Decouple standard spatial sound indicators from absolute positioning.
* **The Constraints:** Implement a sound propagation delay based on physical distance tiles. Gunshots or rapid sprints in an adjacent closed room should sound muffled, heavily distorted, or echo through open structural doorways rather than traveling cleanly through solid concrete walls.
* **Tactical Execution:** Blind fire or pre-firing corners becomes incredibly risky due to unreliable sound origins, forcing reliance on visual confirmation or teammate callouts.

---

## 2. Asymmetric Friction & Weapon Handling Weight

To dismantle the "arcade slide" and twitch-flicking meta, weapons and physical movement must register heavy, momentum-driven inertia variables within the engine code.

### Weapon Weight & Rotational Swivel Lag
* **Mechanical Overhaul:** Introduce a tracking delay between the hardware mouse cursor and the physical orientation vector of the weapon barrel.
* **The Constraints:** * **Lightweight Carbines / Bullpups:** Low swivel lag; quick to bring to bear but limited effective damage over distance.
    * **Heavy Battle Rifles / LMGs:** Massive rotational drag. If a player attempts to snap-flick their mouse $180^{\circ}$, the weapon barrel trails behind the cursor over a heavy, simulated timeline.
* **Tactical Execution:** Prevents high-sensitivity mouse configurations from acting as an instant-win counter against a successful flanking maneuver.

### Movement Inertia & Kinetic Footprint
* **Mechanical Overhaul:** Inject deceleration curves into the player's kinematic velocity functions.
* **The Constraints:** Instantly tapping the opposite directional key should no longer result in an immediate vector snap. Characters must slide or decay their kinetic energy over a noticeable fraction of a second based on their equipped loadout weight class.
* **Tactical Execution:** Over-extending past a doorway makes it impossible to instantly dart back into safety, punishing reckless movement paths.

---

## 3. High-Stakes Biology: Suppression & Hard Stamina

The physical human element must dictate weapon performance under stress. Mechanical precision should degrade rapidly during kinetic friction.

### Asymmetric Vision Suppression
* **Mechanical Overhaul:** Establish a structural "threat weight radius" around projectile paths.
* **The Constraints:** When supersonic rounds pass within a specific pixel distance of a player entity, the rendering viewport must tightly compress, violently shake, or apply an aggressive vignette effect to simulate intense tunnel vision and biological adrenaline.
* **Tactical Execution:** Even if an enemy misses their shots, their volume of fire physically impairs the target's ability to cleanly track targets, making fire superiority and bounding overwatch highly viable tactical doctrines.

### Exhaustive Stamina & Weapon Sway Matrices
* **Mechanical Overhaul:** Connect the character's weapon sway multiplier directly to an active, burning stamina pool.
* **The Constraints:** Sprinting, scaling obstacles, or maintaining an aimed posture with a heavy weapon steadily depletes stamina. When this reservoir runs dry, weapon sway becomes completely unstable, making accurate shot placement over medium ranges functionally impossible.
* **Tactical Execution:** Teams are forced to move at a deliberate walking pace, conserving energy stores before entering hostile sectors.

---

## 4. Large-Scale Operational Logistics & Vehicles

Transitioning away from enclosed, symmetric arenas requires broad, sprawling tactical spaces where traversal becomes an operational issue that demands coordination.

### Vulnerability-Driven Logistics Vehicles
* **Mechanical Overhaul:** Implement multi-seat vehicular entities bound by realistic operational vulnerabilities.
* **The Constraints:** Introduce utility transport trucks or armed technical platforms. These assets require distinct crew allocations (a dedicated driver with restricted vision windows and a gunner exposed to external counter-fire). Vehicles carry a devastating physical presence but are incredibly susceptible to localized component damage (such as disabling a tire or blowing the engine block).
* **Tactical Execution:** Vehicles serve as vital transport mechanisms across vast open zones rather than arcade-style combat platforms.

### Physical Magazine Repacking & Supply Chain
* **Mechanical Overhaul:** Eliminate abstract ammo pools. Ammo must be tracked as individual, physical magazines.
* **The Constraints:** Pressing reload does not magically pool remaining bullets into a fresh magazine. Partially spent magazines are retained in the inventory. To recover maximum capacity, players must find hard cover, halt movement, and execute a lengthy, sequential "magazine repacking" animation loop.
* **Tactical Execution:** Fire discipline becomes paramount. Running dry in the middle of a prolonged engagement forces immediate reliance on a sidearm or fallback cover logistics crates.
