import regex 

class _FromDictMixin:
    """
    usage:
    class A(_FromDictMixin):
        field1 = None
        field2 = "default"
        fields_required = ["field1"]
        
        def set_field1(self, value): # optional
           # make some check
           self.field1 = value
        
        def validate(self, current_depth=""): # thi si is optional
            # place your code de validate all field    
    """
    fields_required = []

    def from_dict(self, d: dict, current_depth=""):
        for k, v in d.items():
            # add support for env(VAR_NAME)
            if isinstance(v, str):
                match = regex.match(r"env\((?P<var_name>[A-Z_]+)\)", v)
                if match:
                    logger.debug(
                        "key %s is an env var. Getting it" % current_depth + "." + k
                    )
                    v = os.getenv(match.group("var_name"), None)
            if hasattr(self, f"set_{k}"):
                # has set_XXX(), then call it
                func = getattr(self, f"set_{k}")
                func(v)
            elif hasattr(self, k):
                # has XXX, then set it
                attr = getattr(self, k)
                if isinstance(v, dict) and isinstance(attr, _FromDictMixin):
                    attr.from_dict(v, current_depth=current_depth + "." + k)
                else:
                    setattr(self, k, v)
        self.validate(current_depth=current_depth)

    def validate(self, current_depth=""):
        errors = []
        for field in self.fields_required:
            value = getattr(self, field, None)
            if value is None:
                errors.append(field)
        if errors:
            fields = ", ".join(errors)
            msg = f"In '{current_depth}' configuration, mandatory fields '{fields}' are missing"
            logger.error(msg)
            raise ValueError(msg)
