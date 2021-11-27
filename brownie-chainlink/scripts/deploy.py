from brownie import MQTTClient, accounts, config, network
from scripts.helpful_scripts import fund_with_link

def deploy():
    account = accounts.add(config["wallets"]["from_key"])
    mqtt_client = MQTTClient.deploy({'from': account})

def main():
    deploy()