TYPE = "Word"
TYPE_FULL = "struct Word"
TYPE_LOW = "word"

with open("vector.h.template", "r") as f:
    header_template = f.read()
with open("vector.c.template", "r") as f:
    source_template = f.read()

header = header_template.replace("{{TYPE}}", TYPE) \
                        .replace("{{TYPE_FULL}}", TYPE_FULL) \
                        .replace("{{TYPE_LOW}}", TYPE_LOW)

source = source_template.replace("{{TYPE}}", TYPE) \
                        .replace("{{TYPE_FULL}}", TYPE_FULL) \
                        .replace("{{TYPE_LOW}}", TYPE_LOW)

with open('vector.output.h', "w") as f:
    f.write(header)
    f.flush()

with open('vector.output.c', "w") as f:
    f.write(source)
    f.flush()
