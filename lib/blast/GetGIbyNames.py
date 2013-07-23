from datetime import datetime

start_time = datetime.now()
gifile = open('necessary_names.txt', 'w+')
namesFile = open("gi_title.txt")
s = namesFile.readlines()
print(len(s))
count=0
for pair in s:
    current_gi, title = pair.split('   ')
    if 'Ig' in title or 'imm' in title or 'VH' in title or 'VL' in title:
        #gifile.writelines(current_gi+'\n')
        gifile.writelines(pair)
        count+=1
gifile.close()

print(count, datetime.now()-start_time)
input()
