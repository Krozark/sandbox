import smtplib
from email.mime.application import MIMEApplication
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText


def _get_attachment_file_nameand_type(path: str) -> Tuple[str, str]:
    base_name = os.path.basename(os.path.basename(path))
    name = regex.match(EMAIL_ATTACHMENT_REGEX_TEMPLATE_FILE_NAME, base_name).group(
        "name"
    )
    t = name.rsplit(".", 1)[-1]
    return name, t


def send_smtp_mail(
    receivers: List[str],
    object: str,
    body: str,
    html: str = None,
    attachments: List[Tuple[str, str]] = None,
) -> None:
    logger.debug("Creating SMTP connexion")
    # create smtp connection
    smtp = smtplib.SMTP(
        host=config.email.smtp.host,
        port=config.email.smtp.port,
        local_hostname=config.email.smtp.local_hostname,
    )
    # Login with your email and password
    if config.email.smtp.password:
        smtp.ehlo()
        smtp.starttls()
        smtp.login(config.email.sender, config.email.smtp.password)

    logger.info(f"Preparing SMTP e-mail to {receivers}:")
    for receiver in receivers:
        logger.info(f"Sending e-mail to {receiver}.")
        msg_root = MIMEMultipart("related")
        msg_root["Subject"] = object
        msg_root["From"] = config.email.sender
        msg_root["To"] = receiver
        msg_root["Date"] = datetime.datetime.now().strftime("%a, %d %b %Y  %H:%M:%S %Z")
        # Set the multipart email preamble attribute value.
        # Please refer https://docs.python.org/3/library/email.message.html to learn more.
        msg_root.preamble = "====================================================="

        # Create a 'alternative' MIMEMultipart object. We will use this object to save plain text format content.
        msg_alternative = MIMEMultipart("alternative")
        msg_root.attach(msg_alternative)

        msg_alternative.attach(MIMEText(body))  # Add text contents
        if html:
            msg_alternative.attach(MIMEText(html, "html"))  # Add html contents

        for attachment_path, attachment_data in attachments or []:  # add attachments
            name, subtype = _get_attachment_file_nameand_type(attachment_path)
            attachment = MIMEApplication(attachment_data, name=name, _subtype=subtype)
            attachment["Content-Disposition"] = f'attachment;filename="{name}"'
            msg_root.attach(attachment)

        # logger.debug("Email to send:\n%s", msg_root.as_string())
        smtp.sendmail(
            from_addr=config.email.sender, to_addrs=[receiver], msg=msg_root.as_string()
        )
    smtp.quit()


def send_mailgun_email(
    receivers: List[str],
    object: str,
    body: str,
    html: str = None,
    attachments: List[Tuple[str, str]] = None,
) -> None:
    logger.info(f"Preparing mailgun e-mail to {receivers}:")
    url = urljoin(config.email.mailgun.url, config.email.mailgun.domain + "/messages")
    auth = ("api", config.email.mailgun.key)
    files = [
        (
            "attachment",
            (
                _get_attachment_file_nameand_type(attachment_path)[0],
                attachment_data,
            ),
        )
        for attachment_path, attachment_data in attachments or []
    ]
    data = {"from": config.email.sender, "subject": object, "text": body}
    if html:
        data["html"] = html

    logger.debug(f"mailgun url {url}, with {len(files)} attachment")

    for receiver in receivers:
        logger.info(f"Sending e-mail to {receiver}.")
        response = requests.post(
            url,
            auth=auth,
            files=files,
            data={
                "to": receiver,
                **data,
            },
        )
        if response.status_code != 200:
            logger.error(
                f"Error while querying url '{url}'. {response.status_code}: {response.text}."
            )
            response.raise_for_status()


def send_mail(
    receivers: List[str],
    object: str,
    body: str,
    html: str = None,
    attachments: List[Tuple[str, str]] = None,
) -> None:
    if config.email.mailgun.is_valid():
        send_mailgun_email(receivers, object, body, html=html, attachments=attachments)
    elif config.email.smtp.is_valid():
        send_smtp_mail(receivers, object, body, html=html, attachments=attachments)
    else:
        msg = "No mail backend configured"
        logger.error(msg)
        raise ValueError(msg)

