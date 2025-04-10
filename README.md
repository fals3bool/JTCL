# JTCL
JTCL (Java Tomcat Launcher) is command line application written in C that helps deploying a java web application locally.

## Download & Install

###  Download bin file
Download `jcatl` from the lastest release.

###  Build it yourself

`linux`
```shell
# need some libs
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
# or
sudo pacman -S ncurses
```

`windows`<br>
```
I dunno... It won't work anyways
```

`build it!! :)`
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

# remove war file & folder from $CATALINA_HOME/webapps
WAR="war_file_without_extension" jcatl remove

# open text user interface (TUI)
WAR="war_file_without_extension" jcatl listen
# you'll see 4 clickable buttons
[ SET ]  [ RUN ]  [ STOP ]  [ REMOVE ]
# exit TUI with q or ctrl+c

# TUI only works on gnome-terminal, xterm, alacritty, Konsole...
# The program without TUI can be found in the first release (may have cache errors)
```

## Author message

This program is just a simple implementation made to simplify and speed up my work. Some IDEs have plugins for this but I couldn't find any that fit my requirements: `just something simple, small & quick`.

Even though it's still improvable, I may not change or update this repo in a while (unless I need it).

`(Los mensajes de la consola están en spanglish)`

