# tests for the tagbucket class
echo ---------------------------------------------------------------------------
echo tagbucket.sh: simple test tagbucket1
rm -f infile outfile
./genfile 100 > infile
./tagbucket1 -iinfile | sort -n > outfile
cmp infile outfile
if [ $? -ne 0 ] ; then
   exit 1
fi
rm -f infile outfile
