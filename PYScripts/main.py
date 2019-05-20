import numpy as np
import matplotlib.pyplot as plt
import sys

from logger import main_logger
import pause
import go_to_formation

def main(list_Ball, list_TeamBlue, list_TeamYellow):
    main_logger.info("Start main script")
    
    # print(list_Ball)
    # print(list_TeamBlue)
    # print(list_TeamYellow)
    position_blue_robot_0 = [list_TeamBlue[12], list_TeamBlue[12 * 2]]
    position_blue_robot_1 = [list_TeamBlue[1 + 12], list_TeamBlue[1 + 12 * 2]]
    position_blue_robot_2 = [list_TeamBlue[2 + 12], list_TeamBlue[2 + 12 * 2]]
    position_blue_robot_3 = [list_TeamBlue[3 + 12], list_TeamBlue[3 + 12 * 2]]
    position_blue_robot_4 = [list_TeamBlue[4 + 12], list_TeamBlue[4 + 12 * 2]]
    position_blue_robot_5 = [list_TeamBlue[5 + 12], list_TeamBlue[5 + 12 * 2]]
    position_blue_robot_6 = [list_TeamBlue[6 + 12], list_TeamBlue[6 + 12 * 2]]
    position_blue_robot_7 = [list_TeamBlue[7 + 12], list_TeamBlue[7 + 12 * 2]]

    position_blue_robots = [position_blue_robot_0,
                            position_blue_robot_1,
                            position_blue_robot_2,
                            position_blue_robot_3,
                            position_blue_robot_4,
                            position_blue_robot_5,
                            position_blue_robot_6,
                            position_blue_robot_7]

    rules = []
    if not pause.PAUSE:
        rules = go_to_formation.go_to_formation_2_alg(position_blue_robots)
    else:
        for x in range(0, 12):
            rule_robot = [x, 0, 0, 0, 0, 0]
            rules.append(rule_robot)

    return rules
