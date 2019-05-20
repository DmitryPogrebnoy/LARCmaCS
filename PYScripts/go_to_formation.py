def go_to_formation_2_alg(position_blue_robots):
        rule_0 = [0] + go_to_point(position_blue_robots[0][0], position_blue_robots[0][1], -4500.0, 0.0)
        rule_1 = [1] + go_to_point(position_blue_robots[1][0], position_blue_robots[1][1], -3600.0, -550.0)
        rule_2 = [2] + go_to_point(position_blue_robots[2][0], position_blue_robots[2][1], -3600.0, 550.0)
        rule_3 = [3] + go_to_point(position_blue_robots[3][0], position_blue_robots[3][1], -645.0, 0.0)
        rule_4 = [4] + go_to_point(position_blue_robots[4][0], position_blue_robots[4][1], -645.0, 2100.0)
        rule_5 = [5] + go_to_point(position_blue_robots[5][0], position_blue_robots[5][1], -645.0, -2100.0)
        rule_6 = [6] + go_to_point(position_blue_robots[6][0], position_blue_robots[6][1], -2000.0, 750.0)
        rule_7 = [7] + go_to_point(position_blue_robots[7][0], position_blue_robots[7][1], -2000.0, -750.0)
        rules = [rule_0, rule_1, rule_2, rule_3, rule_4, rule_5, rule_6, rule_7]
        return rules

def go_to_point(cur_x, cur_y, point_x, point_y):
    command = [0, 0, 0, 0, 0]
    if (abs(cur_x - point_x) <= 40) and (abs(cur_y - point_y) <= 40):
        command = [0, 0, 0, 0, 0]
    elif (abs(cur_x - point_x) <= 40) and (cur_y > point_y) and (abs(cur_y - point_y) > 40):
        command = [0, 10, 0, 0, 0]
    elif (abs(cur_x - point_x) <= 40) and (cur_y < point_y) and (abs(cur_y - point_y) > 40):
        command = [0, -10, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x > point_x) and (abs(cur_y - point_y) <= 40):
        command = [-10, 0, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x > point_x) and (cur_y > point_y) and (abs(cur_y - point_y) > 40):
        command = [-10, 10, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x > point_x) and (cur_y < point_y) and (abs(cur_y - point_y) > 40):
        command = [-10, -10, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x < point_x) and (abs(cur_y - point_y) <= 40):
        command = [10, 0, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x < point_x) and (cur_y > point_y) and (abs(cur_y - point_y) > 40):
        command = [10, 10, 0, 0, 0]
    elif (abs(cur_x - point_x) > 40) and (cur_x < point_x) and (cur_y < point_y) and (abs(cur_y - point_y) > 40):
        command = [10, -10, 0, 0, 0]
    
    return command
