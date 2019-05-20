import logging
import logging.config
import os

path_to_logger_config = os.path.join(os.path.dirname(os.path.abspath(__file__))
                                                         ,"logging.conf")

logging.config.fileConfig(path_to_logger_config)

main_logger = logging.getLogger("main")
pause_logger = logging.getLogger("pause")
go_to_formation_logger = logging.getLogger("go_to_formation")

