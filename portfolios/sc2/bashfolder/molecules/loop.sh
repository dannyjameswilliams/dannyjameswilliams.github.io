
for file in *.pdb
do
  tail -n 1 $file > end_lines.txt
done

