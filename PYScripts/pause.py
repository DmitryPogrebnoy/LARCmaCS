from logger import pause_logger
from importlib import reload
import go_to_formation
import main


PAUSE = True

def pause_unpause():
    reload(go_to_formation)
    reload(main)

    global PAUSE
    
    if PAUSE:
        pause_logger.info("Algorithm pause")
    else:
        pause_logger.info("Algorithm unpause ")
    
    old_PAUSE = PAUSE
    PAUSE = False if PAUSE else True
    
    return PAUSE
    