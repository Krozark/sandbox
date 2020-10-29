import logging

from pyproxy.facades.os import Os
from pyproxy.proxy import ProxyClass

logger = logging.getLogger(__name__)


def main():

    for platform in ("linux", "win"):
        logger.info("Load proxy for %s", platform)
        logger.debug('os_proxy = Proxy(Os, "%s") ->', platform)
        os_proxy = ProxyClass(Os, platform)
        logging.debug("> %s", os_proxy)

        logging.debug("os_class = os_proxy._ensure_object_class() ->")
        os_class = os_proxy._ensure_object_class()
        logging.debug("> %s", os_class)

        logging.debug("os = os_class() ->")
        os = os_class()
        logging.debug("> %s", os)

        logging.debug("os.name: %s", os.name())

    for platform in ("linux", "win"):
        logger.info("Load proxy for %s", platform)
        logger.debug('os = Proxy(Os, "%s")() ->', platform)
        os = ProxyClass(Os, platform)()
        logging.debug("os.name: %s", os.name())


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
