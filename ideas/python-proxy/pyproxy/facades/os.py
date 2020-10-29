import logging

logger = logging.getLogger(__name__)


class Os(object):
    def __init__(self):
        logger.debug("Build facade.os.Os")

    def name(self):
        logger.debug("call name()")
        return self._name_impl()

    def _name_impl(self):
        raise NotImplementedError()
