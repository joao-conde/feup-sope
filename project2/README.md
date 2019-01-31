# PROJECT1: A multi-threaded 'sauna' management application

Multi-threaded application to simulate and manage a sauna with multiple clients of different genders arriving with different service times.

After compiling with make, run first the 'sauna' and then the 'gerador'.


First run the sauna, like this:


```console
user:~$ ./sauna <maximum clients at once - capacity>
```


Then run the generator, like this:


```console
user:~$ ./gerador <users/requests to generate> <maximum duration of each client stay in the sauna (ms)>
```