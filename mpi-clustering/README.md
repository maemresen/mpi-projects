[<<< home](../README.md)
* [MPI BI-Gram Finder](../mpi-bi-gram-finder/README.md)
* MPI Clustering
* [MPI Pi Number Calculation](../mpi-pi-number-calculation/README.md)

# MPI Clustering
A different implementation of the [mpi-bi-gram-finder](../mpi-bi-gram-finder/README.md) project with using a Beowulf cluster.

We have used 2 virtual worker nodes and 1 virtual master node for cluster, all operating with Ubuntu Server 16.04. Each computer in cluster configured for Message Passing Interface and Network File System, Hydra Process Manager is used to manage processes between nodes.

Raspberry Pi is used to create a Access Point for connect different computers on same subnet.



## Cluster Configuration Steps

### 1) Editing host file for all nodes
Following configurations need to be done in **all** nodes \
Add following to /etc/hosts file
```
127.0.0.1 localhost
192.168.42.50 master
192.168.42.51 node1
192.168.42.52 node2
```

### 2) Creating user for running MP jobs
Following configurations need to be done in **all** nodes \
Run following  command

```bash
$ sudo adduser mpiuser --uid 999
```

Every computer need a MPI user with ​ same ​ uuid, because;
* Mpich uses SSH for communication between nodes. Passwordless login is easier with same usernames
and no needed to track authorized keys, all keys shared with NFS shared directory.
* NFS directory is accessible for MPI users only. All MPI users need to have same uuid for NFS
communication.

### 3) Install and setup the Network File System

Network File System (NFS) enables you to mount part of a remote file system so MPI users can access it. Files and programs used for compute bigram analysis needed to be in every node.  \
To install NFS, run the following command on the ​ master ​ node:
```bash
master:~$ sudo apt-get install nfs-kernel-server
```

**All compute nodes**​ needs to be install following package:
```bash
$ sudo apt-get install nfs-common
```

We have used NFS to share ​ ```/home/mpiuser``` ​ directory with compute nodes. It is important that this directory
is owned by the MPI user so that all MPI users can access this directory. But since we created this home directory
with the adduser command earlier, it is already owned by the MPI user.

For checking if the needed directory owned by ​ mpiuser:

```
master:~$ ls -l /home/ | grep mpiuser
drwxr-xr-x
7 mpiuser mpiuser 4096 May 30 05:55 mpiuser
```

If you want to share another directory rather than mpiuser’s home directory, you must change its ownership with
following command:
```bash
master:~$ sudo chown mpiuser:mpiuser /path/to/shared/dir
```

Now we share the ​ ```/home/mpiuser```​ directory ​**of the master node​** with all other nodes. For this the file
```/etc/exports```​ on the master node needs to be edited. Add the following line to this file
```
/home/mpiuser *(rw,sync,no_subtree_check)
```
After the first install you may need to restart the NFS daemon
```bash
master:~$ sudo service nfs-kernel-server restart
```
For testing NFS following command may be useful
```
$ showmount -e master
```
In this case this should print the path ​ ```/home/mpiuser```
We have disabled firewall but, if you want to access from another subnet with enabled firewall, you need to allow
connections with command
```bash
master:~$ sudo ufw allow from 192.168.42.0/24
```

You should then be able to mount ​ master:/home/mpiuser​ on the compute nodes. Run the following
commands to test this,
```bash
node1:~$ sudo mount master:/home/mpiuser /home/mpiuser
node2:~$ sudo mount master:/home/mpiuser /home/mpiuser
```
If this command hangs or fails, you need to check your configuration steps. This configuration can be tested with
creating a new file under ​ ```/home/mpiuser/```

If mounting the NFS shared directory works, we can make it so that the ```master:/home/mpiuser``` directory is
automatically mounted when the compute nodes are booted. For this the file /etc/fstab needs to be edited. Add the
following line to the fstab file of ​ **all** ​ compute nodes
```bash
master:/home/mpiuser /home/mpiuser nfs
```

### 4) Install SSH
SSH needs to be installed if even exists, because we have used Ubuntu Server and sshd service must be accessible
from other nodes. First install the SSH server on ​ all ​ nodes:
```
$ sudo apt-get install ssh
```
In ​ any ​ node, you need to generate SSH keys with ​ ```mpiuser```​ , ​ ​ generated keys automatically shared with other
nodes with NFS.
```
$ su mpiuser
$ ssh-keygen
```
When asked for a passphrase, leave it empty (hence passwordless SSH).
Run the following commands on the ​ master n ​ ode as user ​ mpiuser
```
mpiuser@master:~$ ssh-copy-id localhost
```
If this configuration done correctly, you need to be able to access other nodes with following commands
```
mpiuser@master:~$ ssh node1
mpiuser@node1:~$ echo $HOSTNAME
node1
```

### 4) Setup Hyrdra Process Maanger
Hydra is needed for manage processes between compute nodes. The process manager is included with the MPICH
package, so start by installing MPICH on ​ all ​ nodes with
```
$ sudo apt-get install mpich
```
In order to setup Hydra, we need to create one file on the master node. This file contains all the host names of the
compute nodes
```
mpiuser@master:~$ cd ~
mpiuser@master:~$ touch hosts
```
In order to be able to send out jobs to the other nodes in the network, add the host names of all compute nodes to
the hosts file,
```
node1
node2
```
You may choose to include master in this file, which would mean that the master node would also act as a
compute node.

## Raspberry Pi Access Point Configuration Steps
We have used a Raspberry Pi as a Access Point because we need to connect any other computers to virtual
machines in cluster. Computers connected to Raspberry (with IP 192.168.42.x), can be join to Beowulf Cluster,
they all connected to same subnetwork.


### 1) Edit Interfaces
Add following to ```/etc/network/interfaces``` file
```
auto eth0
iface eth0 inet static
address 192.168.42.1
netmask 255.255.255.0
network 192.168.42.0
broadcast 192.168.42.255
gateway 192.168.42.1
dns-nameservers 192.168.42.1 8.8.8.8
```

### 2) Edit Resolv.conf
We need to add our DNS settings to resolv.conf \
Add the following line to ```/etc/resolv.conf```
```
name server xxx.xxx.xxx.xxx
```
`xxx.xxx.xxx.xxx` is your DNS settings in​ ```/etc/network/interfaces```

### 3) Networking Service Restart
```
$ sudo /etc/init.d/networking restart
```