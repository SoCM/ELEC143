#scrape mbed.org to obtain the names of all projects with the prefix Task - I have 3 pages of repositories
curl "https://developer.mbed.org/users/noutram/code/?page=1&sort=alpha" > weblist.html
curl "https://developer.mbed.org/users/noutram/code/?page=2&sort=alpha" >> weblist.html
curl "https://developer.mbed.org/users/noutram/code/?page=3&sort=alpha" >> weblist.html

#First we prune just the lines with a URL that includes Task
grep -i "code/Task" weblist.html > grep-output.html
#Extract all lines starting with (whitespace) <a href to avoid duplicates
sed -n -e '/^[[:space:]]*<a href.*/p' grep-output.html > filtered.html
#Extract the project name - (spaces + any text + code/) + (any text except a /) + (/ + any text to end of line)
#The second region \2 is the Project name
sed -e 's:\(^[[:space:]]*.*code/\)\([^/]*\)\(/.*\):\2:' filtered.html > foundtasks.txt
#Prune any duplicates
awk '!seen[$0]++' foundtasks.txt > tasks.txt

#Inform user
echo "Found the following tasks"
cat tasks.txt

echo "Press return to continue"
read

#Create a tasks folder
mkdir tasks
cd tasks

#do this ONCE - this is the shared mbed-os library that all projects will use
echo "Importing shared copy of mbed-os"
mbed import https://github.com/ARMmbed/mbed-os

#do the following for each hg repository
while read name
do
    #import mbed project from mbed.org site
    echo "importing $name"
    mbed import "https://developer.mbed.org/users/noutram/code/$name/"

    #create uVision5 project for NUCLEO_F401RE
    echo "Creating uVision5 project for $name"
    cd $name
    mbed target NUCLEO_F401RE
    mbed toolchain GCC_ARM
    mbed export -i uvision5 -m NUCLEO_F401RE

    #Copy uVision5 project file to temporary text file
    cp $name.uvprojx $name.uvprojx.txt

    #replace all instances of mbed-os with ../mbed-os
    sed -e 's:mbed-os:\.\./mbed-os:g' $name.uvprojx.txt > $name.uvprojx

    #Remove local copy of mbed-os
    rm -Rf mbed-os

    #Back up for the next one
    cd ..
done < ../tasks.txt
