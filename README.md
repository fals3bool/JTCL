# JTCL
JTCL (Java Tomcat Launcher) is command line application written in C that helps running a deploying a java web application locally.

## Download & Install

###  Download bin file
Download `jcatl` from the lastest release.

###  Build it yourself
```shell
git clone this-repo
cd build/
cmake ..
make
```

---
### Add it to the Path

`linux`
```shell
sudo nano /etc/profile

# write inside '/etc/profile'
export JTCL_HOME="/home/fabri/Data/code/c/JTCL"
export PATH=$JTCL_HOME/build/bin:$PATH

# Also needs CATALINA_HOME
export CATALINA_HOME="/opt/apache-tomcat"
```

## How to Use

### Build your war file
```shell
mvn clean package
```

### Use JTCL
```shell
# copy target/war_file to $CATALINA_HOME/webapps
WAR="war_file_without_extension" jcatl set

# run tomcat/startup
WAR="war_file_without_extension" jcatl run

# run tomcat/shutdown
WAR="war_file_without_extension" jcatl stop

# remove war file from $CATALINA_HOME/webapps
WAR="war_file_without_extension" jcatl remove
```

## Author message

This program is just a simple implementation made to simplify and speed up my work. Some IDEs have plugins for this but I couldn't find any that fit my requirements: `just something simple, small & quick`.

Even Though it's still improvable, I may not change or update this repo in a while (Only if I need it).

`(Los mensajes de la consola están en spanglish)`

