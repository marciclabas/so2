FROM ubuntu:focal

# Set environment variables for non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Copy and execute bootstrap.sh to provision the container
COPY bootstrap.sh /bootstrap.sh

# Run the bootstrap.sh script
RUN chmod +x /bootstrap.sh && /bootstrap.sh

# Forward X11 for graphical applications (requires additional setup on the host)
# You'll need to mount the X11 socket on the host when running the container
ENV DISPLAY=:0

CMD ["tail", "-f", "/dev/null"]