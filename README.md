# JTCL
JTCL (Java Tomcat Launcher) is a light-weight CLI tool written in C that helps deploying a java web application locally.

No need of `.sh` files or writing long commands. Just one small command and everything is running.

(Just for testing locally)

## Download & Install

###  Download bin file
Download `jtcl` from the lastest release.

###  Build it yourself

`linux`
```shell
# clone this-repo
cmake -S . -B build
cmake --build build
```

`windows`
```shell
install linux :)
# Uses some posix functions, sorry :(
# May work with Cygwin, though
```

---
### Add it to the Path

`linux`
```shell
sudo nano /etc/profile

# write inside '/etc/profile'
export JTCL_HOME="/home/fabri/Data/code/c/JTCL"
export PATH=$JTCL_HOME/build:$PATH
# Although, I recommend moving the executable to another folder, like:
#    /opt/jtcl/bin.

# Also needs CATALINA_HOME
export CATALINA_HOME="/opt/apache-tomcat"
```

## How to Use

```shell
# Inside your java folder where pom.xml & /target/ folder are located.
jtcl clean: Clean tomcat/webapps/ folder.
jtcl build: Build & deploy it into tomcat/webapps/ folder.
jtcl run: Start tomcat.

# Can run multiple commands at once:
jtcl clean build run
```

## Author message

This program is just a simple implementation made to simplify and speed up my work. Some IDEs have plugins for this but I couldn't find any that fit my requirements: `just something simple, small & quick`.

I may not change or update this repo in a while (unless I need it).

`(Alerta de spanglish)`

