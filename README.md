C2B2
====

The Capacitive Cell Balancing Board - a new BMS with new technology.

Design principles
=================

This Battery Management System (BMS) is designed with a couple of principles
in mind.

* Robustness - the boards should survive:
  - being connected in the wrong way (cell negative to board positive and 
    vice versa)
  - having either positive or negative connected first.
  - being accidentally connected across multiple cells, even in reverse
    order.
  - having the inter-cell connections plugged in before or after the
    connection to the cell, and being disconnected 
  'survive' means that it may not function, but it will be still usable.
* Functional - the boards should give as much useful information about the
  cell as possible.
* Low power - minimising operational cell drain.
* Communication - the BMS should be able to get information about the state
  of every cell.
* Autonomy - the cell boards should be able to keep working without having
  to receive commands from the central controller.
* Simplicity:
  - one board design for all the same cells - you shouldn't have to get a
    specific 'positive end' or 'negative end' board.
  - low part count and reduced cost.
  - same basic design should work for multiple cell types (though all cells
    in the string must be the same).
  - same design across multiple types - use the same central controller, and
    allow different cell types across strings (e.g. one LiFePO4 string and
    one LiPo string)

How it works
============

Capacitive charge shuttling is a technique (among many) described in 
this paper:

http://www.mdpi.com/1996-1073/6/4/2149/pdf‎

and has been around since at least 2001 (see Moore, S.W.; Schneider, 
P.J. *A Review of Cell Equalization Methods for Lithium Ion and Lithium 
Polymer Battery Systems*. In Proceedings of the SAE 2001 World Congress, 
Detroit, MI, USA, 5–8 March 2001.).

In its simplest sense, it means connecting a capacitor across the 
terminals of one cell, disconnecting it, and then connecting the 
capacitor across the terminals of the adjacent cell, repeatedly.  The 
higher-voltage cell charges the capacitor, and the capacitor charges the 
lower-voltage cell, until the two are equal.  Several similar circuits can
move charge through the pack until all cell voltages are equal.  Using a
capacitor allows the circuit to be very efficient and waste minimal power.
Depending on the turn on and turn off times of each switch, several hundred
or even thousands of cycles can be made per second.

It is worth pointing out that the circuit works on balancing voltages.
Most cell chemistries have a 'state of charge' versus 'voltage' graph that
is not linear.  For example, lithium ion cells will reach a plateau at about
3.75 to 4.0 volts, where continually applying power does not increase the
voltage but still charges the battery.  In this region, the cell balancing
has less effect - the voltage difference between two cells may be minimal
despite them being at different states of charge.  Once at the same voltage,
though, two adjacent cells will stay balanced fairly well.

If the rate of charge or discharge is too fast, it is possible for the 
balancer to not keep up with differences between cells.  This can be 

There is one possible failure mode which is worth noting.  If two
optorelays in the same row in the schematic - e.g. K1 and K2 - are turned
on simultaneously, it will short out cell 1.  At this point, one or more 
of these devices will blow up to protect the cell.  Because the devices
take time to turn on and turn off, there must be a 'dead time' when both
optorelays in the row are turned off to prevent this short.  Diodes won't
help, because current has to be able to flow both ways (into and out of
the cell).

The job of ensuring this dead time is given to the microprocessor.

Microprocessor
==============

The code for each capacitor drive group is fairly simple:

  1. Turn on optorelays for cell A.
  2. Wait (turn on delay) + (shuttle time)
  3. Turn off optorelays for cell A.
  4. Wait (turn off delay) + (dead time)
  5. Turn on optorelays for cell B.
  6. Wait (turn on delay) + (shuttle time)
  7. Turn off optorelays for cell B.
  8. Wait (turn off delay) + (dead time)

The overall speed of this circuit is:

`( (turn on delay) + (turn off delay)  + (shuttle time) + (dead time) ) * 2`
  
The smaller this time, the faster charge can be shuttled between two
adjacent cells.

The turn on delay and turn off delay are set by the schematics of the part
chosen.

In this design, the shuttle time is fixed.  It must be enough to allow
the capacitor to charge or discharge - if the cell is close in voltage to
the capacitor it will take longer for the voltage to change in the
capacitor.  Yet it must not be too long - once the voltage in the capacitor
is close to the voltage in the cell, very little extra charge will flow;
time would be better spent switching the capacitor across to the other cell.

The shuttle time is the area which offers the most promise for future
circuit improvements.  Ideally, the shuttle time is over when the
voltage difference between the cell and the capacitor is closer than a
certain set threshold, which (too) can be tuned; this requires voltage
sensing circuitry.

The dead time is a small fixed time, chosen to be enough time for the
optorelay to stop conducting current and resist further current flow.  
This too must not be too long - it is time in which nothing is 
happening and cells are not being balanced.  It is suggested that this
time be at least half the turn off delay of the optorelay.
