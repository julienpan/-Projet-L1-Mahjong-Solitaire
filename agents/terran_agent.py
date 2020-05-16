# PAN Julien 17802782
# MOUMINI Nourdine 15601036

# python3 -m pysc2.bin.agent --map DefeatZerglingsAndBanelings --agent terran_agent.TerranAgent --use_feature_units --max_episodes 40

from pysc2.agents import base_agent
from pysc2.lib import actions, features, units

import sys
import numpy

#from absl import app
#from absl import flags

_ALLIANCE_SELF = features.PlayerRelative.SELF
_ALLIANCE_ENEMY = features.PlayerRelative.ENEMY

_X_COORD = features.FeatureUnit.x
_Y_COORD = features.FeatureUnit.y

_ATTACK_SCREEN = actions.FUNCTIONS.Attack_screen.id
_MOVE_SCREEN = actions.FUNCTIONS.Move_screen.id

_PLAYER_RELATIVE = features.SCREEN_FEATURES.player_relative.index
_PLAYER_ENEMY = features.PlayerRelative.ENEMY

_MARINE = units.Terran.Marine
_ZERGLING = units.Zerg.Zergling
_BANELING = units.Zerg.Baneling

#FLAGS = flags.FLAGS

def printf(format, *args):
    sys.stdout.write(format % args)
   
#------------------------------------------------------------------------------------
# PREMIERE SOLUTION : (MOYENNE : 26.25, ECART-TYPE : 12.37)
# Envoie les unités alliés contre une unité ennemi choisi et ainsi de suite

#Première méthode sans utiliser feature_units

"""class TerranAgent(base_agent.BaseAgent):
    def step(self, obs):
        super(TerranAgent, self).step(obs)
        if _ATTACK_SCREEN in obs.observation["available_actions"]:
            player_relative = obs.observation["feature_screen"][_PLAYER_RELATIVE]
            enemy_y, enemy_x = (player_relative == _PLAYER_ENEMY).nonzero() 
            if not enemy_y.any():
                return actions.FUNCTIONS.no_op()                   
            index = numpy.argmax(enemy_y)
            target = [enemy_x[index], enemy_y[index]]
            return actions.FUNCTIONS.Attack_screen("now", target)
        else:
            return actions.FUNCTIONS.select_army("select")

"""
# Deuxième méthode en utilisant feature_units

"""class TerranAgent(base_agent.BaseAgent):
    scores = []
    def reset(self):
        self.scores.append(0)
    def __del__(self):
        print("avg %.2f std %.2f" %(numpy.mean(self.scores),
                                    numpy.std(self.scores)))
    def step(self, obs):
        super(TerranAgent, self).step(obs)
        self.scores[self.episodes-1] = obs.observation["score_cumulative"][0]
        if _ATTACK_SCREEN in obs.observation["available_actions"]:
            enemies = [unit for unit in obs.observation.feature_units
                         if unit.alliance == _PLAYER_ENEMY]
            if len(enemies) == 0:
                return actions.FUNTION.no_op()
            best_i = 0
            best_y = 0
            for i in range(len(enemies)):
                if enemies[i][_Y_COORD] > best_y :
                    best_y = enemies[i][_Y_COORD]
                    best_i = i
            target = [enemies[best_i][_X_COORD], enemies[best_i][_Y_COORD]]
            return actions.FUNCTIONS.Attack_screen("now", target)         
        else:
            return actions.FUNCTIONS.select_army("select")
 """    
#------------------------------------------------------------------------------------                

# DEUXIEME SOLUTION : (MOYENNE : 23.90, ECART-TYPE : 6.17)
# Envoie les unités alliés contre les unités ennemies, chaque unité allié attaque
# l'unité la plus proche d'elle en utilisant le principe de l'attaque automatique

"""class TerranAgent(base_agent.BaseAgent):
    scores = []
    
    def reset(self):
        self.scores.append(0)
        
    def __del__(self):
        print("avg %.2f std %.2f" %(numpy.mean(self.scores),
                                    numpy.std(self.scores)))
                                    
    def step(self, obs):
        super(TerranAgent, self).step(obs)
        self.scores[self.episodes-1] = obs.observation["score_cumulative"][0]
        if _MOVE_SCREEN in obs.observation["available_actions"]:
            player_relative = obs.observation["feature_screen"][_PLAYER_RELATIVE]
            enemy_y, enemy_x = (player_relative == _PLAYER_ENEMY).nonzero()
            if not enemy_y.any():
                return actions.FUNCTIONS.no_op()
            target = [int(enemy_x.mean()), int(enemy_y.mean())]
            return actions.FUNCTIONS.Move_screen("now", target)
        else:
            return actions.FUNCTIONS.select_army("select")
"""
#------------------------------------------------------------------------------------

# TROISIEME SOLUTION : (MOYENNE : 23.05 ECART-TYPE: 14.94 pour d_run = 200.0)

# Cette fois-ci, en utilisant le principe du hit-and-run pour avoir meilleur chance
# de ne pas être touché par l'ennemi

"""class TerranAgent(base_agent.BaseAgent):
    d_run = 200.0
    pos_A = [0,0]
    scores = []
   
    def reset(self):
        d_run = 200.0  
        self.scores.append(0)
    
    
    #def set_d_run(self, new_d_run):
     #   self.d_run = new_d_run
        
    
    #def EvalMc(self, obs, i):
       
            
    def dist(self, xa, ya, xb, yb):
        return ((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya))
    def average_marines_position(self, obs):
        marines = [unit for unit in obs.observation.feature_units
                   if unit.alliance == _ALLIANCE_SELF]
        pos = [1, 1]
        for i in range(len(marines)):
            pos[0] += marines[i][_X_COORD]
            pos[1] += marines[i][_Y_COORD]
            
        if len(marines) != 0:
            return [pos[0]/len(marines), pos[1]/len(marines)]
    
    def closest_enemy_position(self, pos, enemies):
        best_dist = 10000
        best_i = 0
        for i in range(len(enemies)):
            new_dist = self.dist(pos[0], pos[1], enemies[i][_X_COORD], enemies[i][_Y_COORD])
            if new_dist < best_dist :
                best_dist = new_dist
                best_i = i
            return [enemies[best_i][_X_COORD], enemies[best_i][_Y_COORD]]
    
    
    def __del__(self):
        print("avg %.2f std %.2f" %(numpy.mean(self.scores),
                                    numpy.std(self.scores)))


    def step(self, obs):
        super(TerranAgent, self).step(obs)
        self.scores[self.episodes-1] = obs.observation["score_cumulative"][0]

        # Affichage des coordonnées des unités
        _FU = obs.observation.feature_units
        marines = [unit for unit in _FU if unit.unit_type == _MARINE]
        printf("--- %d marines : ", len(marines))    
        for u in marines:
            printf("(%d %d) : ", u[_X_COORD], u[_Y_COORD])
        print("")       
        banelings = [unit for unit in _FU if unit.unit_type == _BANELING]
        printf("--- %d banelings : ", len(banelings))
        for u in banelings:
            printf("(%d %d) ", u[_X_COORD], u[_Y_COORD])
        print("")      
        zerglings = [unit for unit in _FU if unit.unit_type == _ZERGLING]
        printf("--- %d zerglings : ", len(zerglings))
        for u in zerglings:
            printf("(%d %d) ", u[_X_COORD], u[_Y_COORD])
        print("")
        
        if _MOVE_SCREEN in obs.observation["available_actions"]:
            enemies = [unit for unit in _FU
                       if unit.alliance == _ALLIANCE_ENEMY]
            if len(enemies) == 0:
                return actions.FUNCTIONS.no_op()
            pos = self.average_marines_position(obs)
            pos_r = self.closest_enemy_position(pos, enemies)
            if self.dist(pos[0], pos[1], pos_r[0], pos_r[1]) < self.d_run:
                return actions.FUNCTIONS.Move_screen("now", self.pos_A)
            return actions.FUNCTIONS.Attack_screen("now", pos_r)
        else:
            self.pos_A = self.average_marines_position(obs)
            return actions.FUNCTIONS.select_army("select")
"""

#------------------------------------------------------------------------------------

# (TEST) QUATRIEME SOLUTION (MOYENNE : 28.20, ECART-TYPE : 18.56)
# Meme principe du hit-and-run, mais on tuera d'abord les banelings puis les zerglings

class TerranAgent(base_agent.BaseAgent):
    d_run = 200.0
    pos_A = [0,0]
    
    scores = []
    
    def reset(self):
        d_run = 200.0
        self.scores.append(0)
    
    def dist(self, xa, ya, xb, yb):
        return ((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya))
    def average_marines_position(self, obs):
        marines = [unit for unit in obs.observation.feature_units
                   if unit.alliance == _ALLIANCE_SELF]
        pos = [1, 1]
        for i in range(len(marines)):
            pos[0] += marines[i][_X_COORD]
            pos[1] += marines[i][_Y_COORD]
            
        if len(marines) != 0:
            return [pos[0]/len(marines), pos[1]/len(marines)]
    
    def closest_enemy_position(self, pos, enemies):
        best_dist = 10000
        best_i = 0
        for i in range(len(enemies)):
            new_dist = self.dist(pos[0], pos[1], enemies[i][_X_COORD], enemies[i][_Y_COORD])
            if new_dist < best_dist :
                best_dist = new_dist
                best_i = i
            return [enemies[best_i][_X_COORD], enemies[best_i][_Y_COORD]]
    
    def __del__(self):
        print("avg %.2f std %.2f" %(numpy.mean(self.scores),
                                    numpy.std(self.scores)))
    def step(self, obs):
        super(TerranAgent, self).step(obs)   
        
        self.scores[self.episodes-1] = obs.observation["score_cumulative"][0]
        
        # Affichage des coordonnées des unités
        _FU = obs.observation.feature_units
        marines = [unit for unit in _FU if unit.unit_type == _MARINE]
        printf("--- %d marines : ", len(marines))    
        for u in marines:
            printf("(%d %d) : ", u[_X_COORD], u[_Y_COORD])
        print("")       
        banelings = [unit for unit in _FU if unit.unit_type == _BANELING]
        printf("--- %d banelings : ", len(banelings))
        for u in banelings:
            printf("(%d %d) ", u[_X_COORD], u[_Y_COORD])
        print("")      
        zerglings = [unit for unit in _FU if unit.unit_type == _ZERGLING]
        printf("--- %d zerglings : ", len(zerglings))
        for u in zerglings:
            printf("(%d %d) ", u[_X_COORD], u[_Y_COORD])
        print("")
        
        if len(marines) == 0:
            print(obs.observation["score_cumulative"])
        
        if _MOVE_SCREEN in obs.observation["available_actions"]:
            zerglings = [unit for unit in _FU if unit.unit_type == _ZERGLING]
            banelings = [unit for unit in _FU if unit.unit_type == _BANELING]
            enemies = [unit for unit in obs.observation.feature_units
                       if unit.alliance == _ALLIANCE_ENEMY]
            
            if len(zerglings) == 0 and len(banelings) == 0:
                return actions.FUNCTIONS.no_op()
                
            pos = self.average_marines_position(obs)
            if len(banelings) != 0:
                pos_r = self.closest_enemy_position(pos, banelings)
            else:
                pos_r = self.closest_enemy_position(pos, enemies)
            
            if self.dist(pos[0], pos[1], pos_r[0], pos_r[1]) < self.d_run:
                return actions.FUNCTIONS.Move_screen("now", self.pos_A)
            return actions.FUNCTIONS.Attack_screen("now", pos_r)
        else:
            self.pos_A = self.average_marines_position(obs)
            return actions.FUNCTIONS.select_army("select")
    
