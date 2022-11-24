import jinja2
import regex

def render_templates(path_regex: str, context: dict, results: int = None):
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(config.template_dir))

    res = []
    for file in os.listdir(config.template_dir):
        if regex.match(path_regex, file):
            logger.debug(f"Rendering template {file}.")
            path = os.path.join(config.template_dir, file)
            tpl = env.get_template(file)
            render = tpl.render(context)
            res.append((path, render))

    if results == 1:
        return res[0]
    elif results is not None:
        return res[:results]
    return res
