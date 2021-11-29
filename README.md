### IoT-Chainlink-MQTT-Client
*2021 Fall Chainlink Hackathon*

# Key Features
- Chainlink External Adapter MQTT Client template
- Ethereum Smart Contract MQTT Client template
- IoT device template 

## Inspiration
The Fall 2020 Chainlink Hackathon Best Open Project winner [The Open Library Project](https://devpost.com/software/the-open-library-project) really got my gears turning. There happens to be a "Little Free Library" location right by a park my wife and I walk our dog weekly, and I'm reminded of Aram's project every time we pass it. His use of an IoT device, capturing its output through an external adapter, and executing a Smart Contract on that data, was the primary inspiration for this project. 

## What it does
This project allows a Smart Contract to connect to an MQTT Broker, through a Chainlink External Adapter, and publish or subscribe to IoT devices operating on this MQTT Broker. 

With this project, IoT devices can retain existing MQTT communication infrasctructure when integrating into decentralized systems. 

## How we built it
Python Python Python. I used python to write the [CL-EA-MQTT-Client](https://github.com/Briojas/CL-EA-MQTT-Client) external adapter. This adapter was heavinly influenced by (and actually directly forked from) [Tom Hodges CL-EA-python-template](https://github.com/thodges-gh/CL-EA-Python-Template). I also used python to test the smart contracts in Brownie. 

The Smart Contract was written in Solidity, of course, and the IoT device featured in this project was written in c++ as a PlatformIO project. 

## Challenges we ran into
I struggled to get Brownie really working for me. Still not exactly sure where I was going wrong. I was excited to have a python framework for interacting with Smart Contracts, but wish I had spent more time learning this tool earlier on to better utilize it in this project. 

## Accomplishments that we're proud of
I was pretty excited to see data flow from my Smart Contract, to my Oracle, to my Node, across my external adapter, onto the MQTT Broker, and then all the way back. There were a lot of steps where things could (and did) go wrong, but finally getting an output I was expecting got me pretty pumped. 

## What we learned
- Created my first smart contract
- Ran up my first Chainlink node and connected it to the Kovan Testnet
- Wrote and executed jobs on a Chainlink node
- Connected and called external adapter through bridge task

## What's next for IoT-Chainlink-MQTT-Client
I'd really like to:
- get a keeper contract auto-executing on IoT data 
- create an application for both managing IoT devices and generating Smart Contracts that execute on the data they produce.
