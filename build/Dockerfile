FROM centos/systemd
SHELL ["/bin/bash", "-c"]
ENV container docker
ENV PYTHON_VERSION "3.6.3"
RUN yum install -y https://centos7.iuscommunity.org/ius-release.rpm
RUN yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm ; exit 0
RUN yum -y update
RUN yum  install --nogpgcheck -y \
	    bzip2 \
	    boost169.x86_64 \
        bzip2-libs \
	    which \
        gnutls.x86_64 \
        wget
RUN curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.rpm.sh |  bash
RUN yum install -y gitlab-runner sudo
RUN echo "gitlab-runner ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
CMD gitlab-runner register --non-interactive -r oJeb_Cjpszh3xLQQuyN4 -u https://git.trustwave.com/  --custom_build_dir-enabled  --executor=shell   --tag-list=cdcm_install -name testrunner1234
CMD gitlab-runner run