### PBD algorithm

## Setup

- Mass points :

  - Mass 𝒎𝒊, position 𝒙𝒊, velocity 𝒗𝒊
  
- Constraints :

  - e.g. spring(distance) constraint
  
- Just like mass spring systems

- Main difference time stepping method


## Constraints & Constraint Projection
- Operate on “proposed solution” : 𝒑𝟏,…,𝒑𝒏

  - Distance constraint (spring) : 𝑪𝒑𝒊,𝒑𝒋=𝒑𝒊−𝒑𝒋−𝒅

  - The most primitive of geometric constraints, distance based constraints restore mesh edges that extend or compress relative to their rest length

- Projection

  - Task : update state 𝒑𝟏,…,𝒑𝒏so that the constraints are approximately satisfied

  - PBD uses local linearization : one constraint at a time

  - PBD use only first derivatives
  
## Constraint Projection

-Main idea of PBD

  - Project constraints one by one

  - One descent step per constraint

- Why this works better?

  - 𝒑𝒊are immediately updated

  - Next linearization works with the updated state
  
## Constraint Projection Math
- Taylor expansion :
  - 𝑪𝒑+Δ𝒑≈𝑪𝒑+𝜵𝒑𝑪𝒑∙Δ𝒑=𝟎
  
- Step :

  - Δ𝒑=𝝀𝜵𝒑𝑪𝒑

Putting together :
  - Δ𝒑=−𝑪(𝒑)/|𝜵𝒑𝑪(𝒑)|^𝟐*𝜵𝒑𝑪𝒑
  
  
## Other Constraint : Bending

- Important for cloth and thin shells

- How to measure bending?

  - Simple way : cross spring

  - Drawback : interferes with in plane stretching
  
## Pressure Constraints

- Constraint on volume of closed mesh

  - e.g. Cloth balloons

  - ![image](https://user-images.githubusercontent.com/78521423/191070118-bf95c45a-c494-41ed-bc35-80f7e84874c1.png)
 
      - 𝒗𝟎: original volume

      - 𝒌𝒑𝒓𝒆𝒔𝒔𝒖𝒓𝒆: over pressure factor
      
      
## Collision Constraints

- Assume : collisions already detected

  - Simple objects : easy

- Similar as penalty forces (but constraints)
