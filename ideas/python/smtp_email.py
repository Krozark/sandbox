import smtplib
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def send_mail(receivers, object, body, html=None, attachments=None):
    """
    you will need to provide some infos such as:

     "email": {
      "sender": "your email",
      "password": "password",
      "host": "smtp.gmail.com",
      "port": 587,
      "local_hostname": null
    },
    """
    logger.debug("Creating SMTP connexion")
    # create smtp connection
    smtp = smtplib.SMTP(
        host=config.email.host,
        port=config.email.port,
        local_hostname=config.email.local_hostname,
    )
    # Login with your email and password
    if config.email.password:
        smtp.ehlo()
        smtp.starttls()
        smtp.login(config.email.sender, config.email.password)

    logger.debug(f"Preparing e-mail to {receivers}:")
    for receiver in receivers:
        logger.debug(f"Sending e-mail to {receiver}.")
        msg_root = MIMEMultipart("related")
        msg_root["Subject"] = object
        msg_root["From"] = config.email.sender
        msg_root["To"] = receiver
        msg_root["Date"] = datetime.datetime.now().strftime("%a, %d %b %Y  %H:%M:%S %Z")
        # Set the multipart email preamble attribute value. Please refer https://docs.python.org/3/library/email.message.html to learn more.
        msg_root.preamble = "====================================================="

        # Create a 'alternative' MIMEMultipart object. We will use this object to save plain text format content.
        msg_alternative = MIMEMultipart("alternative")
        msg_root.attach(msg_alternative)

        msg_alternative.attach(MIMEText(body))  # Add text contents
        if html:
            msg_alternative.attach(MIMEText(html, "html"))  # Add html contents

        for attachment_path, attachment_data in attachments or []:  # add attachments
            base_name = os.path.basename(os.path.basename(attachment_path))
            name = regex.match(
                EMAIL_ATTACHMENT_REGEX_TEMPLATE_FILE_NAME, base_name
            ).group("name")
            attachment = MIMEApplication(
                attachment_data, name=name, _subtype=name.rsplit(".", 1)[-1]
            )
            attachment["Content-Disposition"] = f'attachment;filename="{name}"'
            msg_root.attach(attachment)

        # logger.debug("Email to send:\n%s", msg_root.as_string())
        smtp.sendmail(
            from_addr=config.email.sender, to_addrs=[receiver], msg=msg_root.as_string()
        )
    smtp.quit()
