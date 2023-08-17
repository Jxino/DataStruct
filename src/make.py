# Makefile을 읽어서 Makefile에 정해진 rule에 따라 make를 수행한다
import sys
import os
import subprocess
import inspect
# rules = [rule: target, conditions, comands, rule, rule]

def debug(str):
    # 현재 라인 번호와 파일명을 얻기 위해 inspect를 사용합니다.
    current_frame = inspect.currentframe()
    try:
        caller_frame = current_frame.f_back
        filename = caller_frame.f_code.co_filename
        line_number = caller_frame.f_lineno
        print(f"{filename},{line_number}: %s" % (str))
    finally:
        del current_frame

def run_command(command_string):
    try:
        # subprocess.run() 함수를 사용하여 명령어 실행
        result = subprocess.run(command_string, shell=True, capture_output=True, text=True)
        print(result.stdout)
        print(result.stderr)
        if (len(result.stderr) > 0):
            print("run_command() abort. error encountered.")
            sys.exit(0)
    except Exception as e:
        print(f"An error occurred: {e}")
        return None

def read_makefile(Makefile):
    fp = open(Makefile, "r")
    rules = []
    rule = None
    for line in fp:
        if (len(line) <= 1):
            continue
        if (line[0] != '\t'):
            if (rule != None):
                rules.append(rule)
                rule = None
            rule = {"target":"", "conditions":[], "commands":[]}
            target_conditions = line.strip().split(":")
            rule["target"] = target_conditions[0].strip()
            if (len(target_conditions) > 1):
                conditions = target_conditions[1].strip().split(" ")
            rule["conditions"] = conditions
        elif line[0] == '\t':
            rule["commands"].append(line.strip())
    if (rule != None):
        rules.append(rule)
    fp.close()
    return rules

def run_rules(rules, target): # .exe
    for rule in rules:
        if (rule["target"] != target):
            continue
        # debug("-")
        contditions = rule["conditions"]
        if not os.path.exists(target):
            t_mtime = 0
        else:
            t_mtime = os.path.getmtime(target)
        needs_run_commands = False
        # debug("-")
        for condition in contditions:
            run_rules(rules, condition)
            # debug("-")
            c_mtime = 0
            if os.path.exists(condition):
                c_mtime = os.path.getmtime(condition)
            # debug("%d, %d" % (t_mtime, c_mtime))
            if t_mtime < c_mtime:
                needs_run_commands = True
                break
        # debug("-")
        if needs_run_commands:
            for command in rule["commands"]:
                print("running command: %s..." % (command))
                run_command(command)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 %s target [-f Makefile]" % (sys.argv[0]))
        print("       python3 %s all" % (sys.argv[0]))
        sys.exit(0)
    target = sys.argv[1]
    Makefile = "Makefile"
    for i in range(0, len(sys.argv)):
        if (sys.argv[i] == "-f"):
            Makefile = sys.argv[i + 1]
    rules = read_makefile(Makefile)

    # for rule in rules:
    #     print("target: %s, conditions: %s" % (rule["target"], " ".join(rule["conditions"])))
    #     print("commands: \n\t%s\n" % ("\n\t".join(rule["commands"])))

    run_rules(rules, target)