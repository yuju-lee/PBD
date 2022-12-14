# PBD algorithm

## Setup

- Mass points :

  - Mass ππ, position ππ, velocity ππ
  
- Constraints :

  - e.g. spring(distance) constraint
  
- Just like mass spring systems

- Main difference time stepping method


## Constraints & Constraint Projection
- Operate on βproposed solutionβ : ππ,β¦,ππ

  - Distance constraint (spring) : πͺππ,ππ=ππβππβπ

  - The most primitive of geometric constraints, distance based constraints restore mesh edges that extend or compress relative to their rest length

- Projection

  - Task : update state ππ,β¦,ππso that the constraints are approximately satisfied

  - PBD uses local linearization : one constraint at a time

  - PBD use only first derivatives
  
## Constraint Projection

-Main idea of PBD

  - Project constraints one by one

  - One descent step per constraint

- Why this works better?

  - ππare immediately updated

  - Next linearization works with the updated state
  
## Constraint Projection Math
- Taylor expansion :
  - πͺπ+Ξπβπͺπ+π΅ππͺπβΞπ=π
  
- Step :

  - Ξπ=ππ΅ππͺπ

Putting together :
  - Ξπ=βπͺ(π)/|π΅ππͺ(π)|^π*π΅ππͺπ
  
  
## Other Constraint : Bending

- Important for cloth and thin shells

- How to measure bending?

  - Simple way : cross spring

  - Drawback : interferes with in plane stretching
  
## Pressure Constraints

- Constraint on volume of closed mesh

  - e.g. Cloth balloons

  - ![image](https://user-images.githubusercontent.com/78521423/191070118-bf95c45a-c494-41ed-bc35-80f7e84874c1.png)
 
      - ππ: original volume

      - πππππππππ: over pressure factor
      
      
## Collision Constraints

- Assume : collisions already detected

  - Simple objects : easy

- Similar as penalty forces (but constraints)
