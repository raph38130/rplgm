
{
	ByteArrayOutputStream baos;
	int starting_address;
	OutputStream root;

		root = r;
		starting_address = address;
		baos = new ByteArrayOutputStream();

	public void write(byte[] buff, int start, int length)
	{
		baos.write(buff, start, length);
	}

	public void write(byte[] buff)
	{
		baos.write(buff, 0, buff.length);
	}

	public void write(int x)
	{
		baos.write(x);
	}
	{
		try
		{
			if (baos != null)
			{
				byte[] output = baos.toByteArray();
				int address = starting_address;
				int lastaddress = address;
				baos = null;
			}
		}
		catch(Exception e)
		{
			System.out.println("Exception writing TBIN output: "+e);
		}
	}

	public void close()
	{
		flush();
	}
