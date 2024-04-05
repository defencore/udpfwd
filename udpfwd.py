import socket
import argparse
import sys

# Parse command-line arguments
parser = argparse.ArgumentParser(description="UDP stream forwarder")
parser.add_argument("-S", "--source", metavar="IP:PORT", type=str, help="Specify the IP and UDP port to listen on (format: IP:PORT)")
parser.add_argument("-D", "--destination", metavar="IP:PORT", type=str, action='append', help="Specify the IP and UDP port of the destination to forward to (format: IP:PORT)")
args = parser.parse_args()

# Display help if neither -S nor -D parameters are provided
if not (args.source and args.destination):
    parser.print_help(sys.stderr)
    sys.exit(1)

# Parse UDP IP and port from command-line argument
src_ip, src_port = args.source.split(':')
src_port = int(src_port)

# Parse destinations from command-line arguments
DESTINATIONS = []
if args.destination:
    for dst in args.destination:
        dst_ip, dst_port = dst.split(':')
        DESTINATIONS.append((dst_ip, int(dst_port)))

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the server details
sock.bind((src_ip, src_port))

print(f"UDP server started and listening on {src_ip}:{src_port}")
print("Forwarding to destinations:", DESTINATIONS)

# Continuously listen for incoming packets
while True:
    data, addr = sock.recvfrom(65507)  # buffer size, 65507 is maximum UDP packet size
    print("Received", len(data), "bytes from", addr)

    # Forward the received packet to all destinations
    for dst_ip, dst_port in DESTINATIONS:
        sock.sendto(data, (dst_ip, dst_port))
        print("Forwarded to", dst_ip, ":", dst_port)
