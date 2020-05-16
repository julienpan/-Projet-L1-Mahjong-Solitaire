--- Basic Bot Terran ---

Description

A map with 9 Marines on the opposite side from a group of 6 Zerglings and 4 Banelings. 
Rewards are earned by using the Marines to defeat Zerglings and Banelings. 
Whenever all Zerglings and Banelings have been defeated, a new group of 6 Zerglings and 4 Banelings is spawned and the player is 
awarded 4 additional Marines at full health, with all other surviving Marines retaining their existing health (no restore). 
Whenever new units are spawned, all unit positions are reset to opposite sides of the map.

Initial State

9 Marines in a vertical line at a random side of the map (preselected)
6 Zerglings and 4 Banelings in a group at the opposite side of the map from the Marines
Rewards
Zergling defeated: +5
Baneling defeated: +5
Marine defeated: -1
End Conditions
Time elapsed
All Marines defeated
Time Limit
120 seconds
Additional Notes
Fog of War disabled
No camera movement required (single-screen)

This map and DefeatRoaches are currently the only maps in the set that can include an automatic, mid-episode state change for
player-controlled units. The Marine units are automatically moved back to a neutral position (at a random side of the map opposite 
the Roaches) when new units are spawned, which occurs whenever the current set of Zerglings and Banelings is defeated. 
This is done in order to guarantee that new units do not spawn within combat range of one another.
