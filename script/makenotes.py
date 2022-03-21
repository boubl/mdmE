file = open("notes.txt", "r")
output = ""
output2 = "const map<string, string> names = {"
output3 = ""
lines = [line.strip("\n") for line in file if line != "\n"]
i = 0
for line in lines:
    entries = line.split(" ")
    value = entries[0]
    name = " ".join(entries[1:])
    varname = name.replace("-", "_").replace("(","").replace(")","").replace(" ", "")
    if (varname == "Note"):
        varname += "_"
    if (varname == "music"):
        continue
    output += 'const Note Note::' + varname + '() { return Note("' + value + '"); }\n'
    output2 += '\n\t{"' + value + '", "' + name + '"}'
    if (i < len(lines)-2):
        output2 += ','
    else:
        output2 += '\n'
    output3 += 'const static Note ' + varname + '();\n'
    i += 1
output2 += "};\n"
file.close()
file = open("notes.cpp", "w")
file.flush()
file.write(output)
file.write(output2)
file.write(output3)
file.close()