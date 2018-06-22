import optparse
import os

parser = optparse.OptionParser()

parser.add_option('-i', '--input', action="store", dest="input_template", help="Path to the c++ template", default="../proto/sca_cpp_proto.cpp")
parser.add_option('-o', '--output', action="store", dest="output_source", help="Path to the output c++ source", default="../source/sca_cpp.cpp")

options, args = parser.parse_args()

if options.input_template == None or options.output_source == None:
    print (parser.print_help())

with open(options.input_template,'r') as fl:
    lines = fl.readlines()


issue_num = 1
line_num = 1
out_lines = []
for line in lines:
    line = line.replace("\t","    ")
    if line.find("{autonum}") >0  or line.find("{number_issues}") > 0:
        line = line.replace("{autonum}", str(issue_num))
        line = line.replace("{line}", str(line_num))
        if line.find("{number_issues}") > 0:
            line = line.replace("{number_issues}", str(issue_num))
        issue_num += 1
    out_lines.append(line)
    line_num+=1

with open(options.output_source,'w') as fl:
    fl.writelines(out_lines)

print ("The template: " + os.path.abspath(options.input_template))
print ("The result: " + os.path.abspath(options.output_source))

