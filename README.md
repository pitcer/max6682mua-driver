# MAX6682MUA driver

## How to run

* install [Renode](https://renode.io/#downloads)
* `git clone https://github.com/pitcer/max6682mua-driver`
* `cd max6682mua-driver`
* `make run`

## How to test

In renode window:

* login with username `root`
* `insmod maximtemp_driver.ko`
* `./maximtemp`

You should get `21.000000` as that value was set in renode script.
