
last=$(tail -1 mp.txt)
echo $last
sec=$(echo $last | sed 's/.*://g')
echo $sec
fir=$(echo $last | sed 's/:.*//g')
echo $fir

