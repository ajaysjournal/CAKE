#!/usr/bin/env bash


FIRST_ARGUMENT="$1"
echo "Hello, world $FIRST_ARGUMENT!"

echo "Running Genetic Algorithm Test Script"
read NAME
echo "Hello, $NAM E.  Nice to meet you.
\"Today\", I am going to exaplain how to work in python script #"

PI=3.141592654
RAD=7
AREA=$(perl -e "print \"The value is \".($PI * ($RAD*$RAD)).\"\n\";")
echo $AREA

# always execute
if true; then
    ls
else
    echo "true is false."
fi
# never execute
if false; then
    ls
fi


echo "Enter a :"
read A
if [ "$A" = "foo" ] ; then
    echo "Foo"
elif [ "$A" = "bar" ] ; then
    echo "Bar"
else
    echo "Other"
fi


for i in *.out ; do
    mv "$i" "$(echo $i | sed 's/\.out$/.x/')"
    mv "$(echo $i | sed 's/\.out$/.x/')" "$(echo $i | sed 's/\.out$/.out/')"
done