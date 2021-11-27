from brownie import MQTTClient, config, network
from scripts.helpful_scripts import fund_with_link, get_account


def main():
    account = get_account()
    mqtt_client = MQTTClient[-1]
    tx = fund_with_link(
        mqtt_client.address, amount=config["networks"][network.show_active()]["fee"]
    )
    tx.wait(1)
    request_tx = mqtt_client.pub_string(
        'publish',
        '/smart-contract-client/test',
        0,
        'testMessage',
        True,
        {"from": account}
    )
    request_tx.wait(1)
    print(mqtt_client.get_string())
